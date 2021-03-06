#ifndef DATABASE_MANAGER
#define DATABASE_MANAGER
#include "Database/IDatabaseManager.hpp"

#include <fstream>
namespace maycached {
namespace logic {
class Storage;
class ITimeExpirationManager;
class DataMarshallerRawBinary;
class DatabaseManager: public IDatabaseManager
{
public:
    DatabaseManager(std::unique_ptr<DataMarshallerRawBinary> dataMarshaller): m_DataMarshaller(std::move(dataMarshaller))
    {}

    virtual bool searchForValueAndExpiration(uint64_t offset, uint32_t fileId, std::string& value, std::optional<std::chrono::time_point<std::chrono::system_clock>>& willBeExpired) const override;
    std::fstream serializeToFile(const Storage&) const;
    std::unique_ptr<Storage> restoreFromFile(ITimeExpirationManager* timeManager)  const;
private:
    std::unique_ptr<DataMarshallerRawBinary> m_DataMarshaller;
    uint32_t fileId = 0;
};


} }
#endif //I_DATABASE_MARSHALLER
