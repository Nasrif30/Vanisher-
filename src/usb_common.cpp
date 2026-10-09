#include "ewr/usb.h"

#include <algorithm>
#include <cctype>

namespace ewr {

    namespace {

        std::string NormalizeIdentity(const std::string& value)
        {
            std::string normalized;
            normalized.reserve(value.size());

            for (unsigned char ch : value)
            {
                if (std::isalnum(ch))
                    normalized.push_back(static_cast<char>(std::toupper(ch)));
            }

            return normalized;
        }

    } // namespace

    bool IdentityMatchesModel(const std::string& identity, const std::string& expectedModel)
    {
        const std::string normalizedIdentity = NormalizeIdentity(identity);
        const std::string normalizedModel = NormalizeIdentity(expectedModel);

        return !normalizedModel.empty()
            && normalizedIdentity.find(normalizedModel) != std::string::npos;
    }

} // namespace ewr
