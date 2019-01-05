#include <LogicController.hpp>
#include <Handlers/StupidHandler.hpp>
#include <Handlers/VersionHandler.hpp>
#include <Handlers/GetHandler.hpp>
#include <Handlers/SetHandler.hpp>
#include <Handlers/DeleteHandler.hpp>
#include <Storage.hpp>
#include <TimeExpirationManager.hpp>

namespace maycached {
namespace logic {

LogicController::LogicController()
{
    m_TimeExpirationManager = std::make_unique<TimeExpirationManager>();
    m_Storage = std::make_unique<Storage>(m_TimeExpirationManager.get());
    m_TimeExpirationManager->startRepetitivelyRemoveExpired();
    m_FirstHandler = buildChainOfHandlers(*m_Storage);

}

LogicController::~LogicController()
{
    m_TimeExpirationManager->stopRepetitivelyRemovingExpired();
}



std::unique_ptr<IHandler> LogicController::buildChainOfHandlers(IStorage& storage)
{
    auto echoHandler = std::make_unique<StupidHandler>();
    auto versionHandler = std::make_unique<VersionHandler>();
    auto setHandler = std::make_unique<SetHandler>(storage);
    auto getHandler = std::make_unique<GetHandler>(storage);
    auto delHandler = std::make_unique<DeleteHandler>(storage);

    echoHandler->addHandlerToChain(std::move(versionHandler));
    echoHandler->addHandlerToChain(std::move(setHandler));
    echoHandler->addHandlerToChain(std::move(getHandler));
    echoHandler->addHandlerToChain(std::move(delHandler));

    return echoHandler;
}

void LogicController::handleCommand(ICommand &command)
{
    m_FirstHandler->handle(command);
}

} }
