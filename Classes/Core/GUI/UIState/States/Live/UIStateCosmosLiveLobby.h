#pragma once

#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include <Core/GameLogic/Authentication/Authenticator.h>
#include <Core/GameLogic/Live/CosmosLiveCoordinator.h>

#include <Core/GUI/Widgets/Loading/LoadingWidget.h>
#include <Core/GUI/Widgets/Live/Closed/CosmosLiveClosedWidget.h>
#include <Core/GUI/Widgets/Live/PreGameLobby/CosmosLivePreGameLobbyWidget.h>

class UIStateCosmosLiveLobby : public BaseStateDepricated, IAuthenticationResultListener, ICosmosLiveSessionUpdateListener, ICosmosLiveChatReceiver {

public:
    UIStateCosmosLiveLobby(IStateChanageListenerDepricated* stateChangeListener) :
      BaseStateDepricated(stateChangeListener), m_loadingWidget(nullptr), m_closedWidget(nullptr), m_preGameLobbyWidget(nullptr), m_activeState(CosmosLiveState::INVALID) {
      };
    
    ~UIStateCosmosLiveLobby() {
    }
    
    
    CONST_STRING_DEC(UI_STATE_COSMOS_LIVE_LOBBY)

    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual void OnAuthenticationResultReceived(AuthenticationResult result);
    virtual void OnCosmosLiveSessionUpdated(const CosmosLiveSession& session);
    
    virtual void ChatReceived(const std::string& chat);
    
    virtual STRING_ID GetStateID() { return UI_STATE_COSMOS_LIVE_LOBBY; }

private:
    void SubmitGuestLoginRequest();
    void OnMainMenuItemSelected(CosmosLiveClosedWidget::MenuItems selectedItem);
    void OnMainMenuItemSelected(CosmosLivePreGameLobbyWidget::MenuItems selectedItem);
    
    void UpdateCurrentSession(const CosmosLiveSession& session);
    void UpdatePreGameLobby(const CosmosLiveSession& session);
    void ChangeCurrentSession(const CosmosLiveSession& session);
    
    void DisplayLoading();
    void TakeDownLoading();
    
    void DisplayClosed();
    void TakeDownClosed();
    
    void DisplayPreGameLobby();
    void TakeDownPreGameLobby();
    
    void TakeDownAllWidgets();
    
    void DeactivateState(CosmosLiveState state);
    void ActivateState(const CosmosLiveSession& session);

    Authenticator m_authenticator;
    CosmosLiveCoordinator m_cosmosLiveCoordinator;
    
    CosmosLiveState m_activeState;
    LoadingWidget* m_loadingWidget;
    CosmosLiveClosedWidget* m_closedWidget;
    CosmosLivePreGameLobbyWidget* m_preGameLobbyWidget;
};
