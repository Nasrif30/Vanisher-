#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "banner.h"

#include "ewr/executor.h"
#include "ewr/generator.h"
#include "ewr/usb.h"

namespace {

    const std::vector<std::string> kSupportedModels = { "L3250", "L4260" };

    std::string ToUpper(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(std::toupper(ch));
        });
        return value;
    }

    bool IsSupportedModel(const std::string& model)
    {
        return std::find(kSupportedModels.begin(), kSupportedModels.end(), model)
            != kSupportedModels.end();
    }

    void PrintUsage(const char* executable)
    {
        std::cout << "VANISHER | Epson Counter Reset\n"
                  << "Created by ALNASRIF HALIDDIN\n"
                  << "Usage: " << executable << " [--dry-run] [L3250|L4260]\n"
                  << "  --dry-run  Validate the local profile without accessing USB.\n";
    }

    std::string SelectModel()
    {
        while (true)
        {
            std::cout << "Select your printer:\n"
                      << "  [1] Epson L3250\n"
                      << "  [2] Epson L4260\n"
                      << "  [0] Exit\n"
                      << "> ";

            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1")
                return "L3250";
            if (choice == "2")
                return "L4260";
            if (choice == "0")
                return {};

            std::cout << "Invalid selection. Please try again.\n\n";
        }
    }

    const ewr::DbPrinterModel* FindProfile(
        const std::vector<ewr::DbPrinterModel>& models,
        const std::string& name)
    {
        const auto found = std::find_if(models.begin(), models.end(), [&](const auto& model) {
            return model.name == name;
        });
        return found == models.end() ? nullptr : &*found;
    }

    bool MatchesReviewedProfile(const ewr::DbPrinterModel& model)
    {
        // Fail closed if database.json is replaced or updated with values that
        // have not been reviewed for this focused build.
        const std::vector<uint16_t> expectedAddresses = {
            28, 52, 53, 54, 55, 255,
            47, 48, 49, 50, 51, 252, 253, 254,
        };
        const std::vector<uint8_t> expectedResetValues = {
            0, 0, 0, 94, 94, 94,
            0, 0, 0, 0, 0, 0, 0, 0,
        };

        const std::string expectedWriteKey = model.name == "L3250"
            ? "Nbsjcbzb"
            : model.name == "L4260" ? "Bsboujgp" : "";

        return model.rkey == 13898
            && model.wkey == expectedWriteKey
            && model.GetAllAddresses() == expectedAddresses
            && model.GetAllResetValues() == expectedResetValues;
    }

    void PrintProfileSummary(const ewr::DbPrinterModel& model)
    {
        std::cout << "\nReviewed local profile: Epson " << model.name << "\n";
        for (const auto& group : model.pad_groups)
        {
            std::cout << "  - " << group.description << ": "
                      << group.addresses.size() << " EEPROM bytes\n";
        }
        std::cout << "  Total: " << model.GetAllAddresses().size()
                  << " planned EEPROM byte writes\n";
    }

} // namespace

int main(int argc, char** argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    bool dryRun = false;
    std::string selectedModel;

    for (int i = 1; i < argc; ++i)
    {
        const std::string argument = argv[i];
        if (argument == "--dry-run")
        {
            dryRun = true;
        }
        else if (argument == "--help" || argument == "-h")
        {
            PrintUsage(argv[0]);
            return 0;
        }
        else if (selectedModel.empty())
        {
            selectedModel = ToUpper(argument);
        }
        else
        {
            std::cerr << "Unexpected argument: " << argument << "\n";
            PrintUsage(argv[0]);
            return 2;
        }
    }

    std::cout << kVanisherBanner
              << "================================================\n"
              << " VANISHER | Epson Counter Reset\n"
              << " Created by ALNASRIF HALIDDIN\n"
              << " Epson L3250 / L4260 - Direct USB\n"
              << "================================================\n\n";

    if (selectedModel.empty())
        selectedModel = SelectModel();

    if (selectedModel.empty())
        return 0;

    if (!IsSupportedModel(selectedModel))
    {
        std::cerr << "Unsupported model: this build accepts only L3250 and L4260.\n";
        return 2;
    }

    ewr::UniversalGenerator generator;
    if (!generator.LoadDatabase("database.json"))
    {
        std::cerr << "Could not load database.json. Run VANISHER from the folder containing database.json.\n";
        return 1;
    }

    const auto models = generator.GetAvailableModels();
    const ewr::DbPrinterModel* profile = FindProfile(models, selectedModel);
    if (profile == nullptr || !profile->HasResettableCounters())
    {
        std::cerr << "The local " << selectedModel << " profile is missing or incomplete. Nothing was written.\n";
        return 1;
    }

    if (!MatchesReviewedProfile(*profile))
    {
        std::cerr << "The local " << selectedModel
                  << " profile differs from the reviewed version. Nothing was written.\n";
        return 1;
    }

    PrintProfileSummary(*profile);
    const auto sequence = generator.GenerateSequence(*profile);
    const size_t writeCount = static_cast<size_t>(
        std::count_if(sequence.begin(), sequence.end(), ewr::IsWritePacket));

    if (writeCount != profile->GetAllAddresses().size() || writeCount == 0)
    {
        std::cerr << "The generated sequence does not match the profile. Nothing was written.\n";
        return 1;
    }

    if (dryRun)
    {
        std::cout << "\nDRY-RUN OK: " << sequence.size() << " packets, "
                  << writeCount << " planned EEPROM writes. USB was not accessed.\n";
        return 0;
    }

    std::cout << "\nWARNING:\n"
              << "  This resets electronic counters; it does not clean or replace the waste-ink pads.\n"
              << "  Service or replace the pads before continuing to prevent ink leakage.\n"
              << "  This is an unofficial utility. A matching profile does not prove hardware compatibility.\n"
              << "  Use a direct USB cable. Do not turn off the printer during the operation.\n\n"
              << "To authorize EEPROM writes, type exactly: RESET " << selectedModel << "\n"
              << "Enter anything else to cancel.\n> ";

    std::string confirmation;
    std::getline(std::cin, confirmation);
    if (confirmation != "RESET " + selectedModel)
    {
        std::cout << "Reset cancelled. Nothing was changed.\n";
        return 0;
    }

    std::cout << "\nLooking for an Epson " << selectedModel << " over USB...\n";
    ewr::EwrDeviceHandle printer = ewr::AutoConnectEpsonPrinter(selectedModel);
    if (!printer)
    {
        std::cerr << "No Epson " << selectedModel
                  << " with a confirmed USB identity was found. Nothing was written.\n";
        return 1;
    }

    const bool resetOk = ewr::ExecutePayloadSequence(printer, sequence);
    ewr::DisconnectPrinter(printer);

    if (!resetOk)
    {
        std::cerr << "\nRESET NOT CONFIRMED. Check ewr_trace.log; do not retry before reviewing the error.\n";
        return 1;
    }

    std::cout << "\nRESET ACKNOWLEDGED: all " << writeCount
              << " writes received acknowledgements from the printer.\n"
              << "Counter values were not read back by this utility.\n"
              << "Turn the printer off using its physical power button, wait 10 seconds, then turn it on.\n";
    return 0;
}
