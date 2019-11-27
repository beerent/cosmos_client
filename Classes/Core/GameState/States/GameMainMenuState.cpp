#include "Core/GameState/States/GameMainMenuState.h"
#include "IEngine.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"

CONST_STRING_DEF(GameMainMenuState, MAIN_MENU_STATE)
CONST_STRING_DEF(GameMainMenuState, LEVEL_NAME)

void GameMainMenuState::OnEnterState() {
    if (LevelFactory::getInstance()->LevelIsSet() == false) {
        LevelFactory::getInstance()->clearCurrentLevel();
        IEngine::getEngine()->ResetPhysics();
        LevelFactory::getInstance()->LoadLevel(StringManager::getIDForString("KYCPlanetSizeLevel"));
    }
	UIStateMachine* uiStateMachine = UIStateMachine::GetInstance();
	uiStateMachine->SetEventHandler(*this);
	uiStateMachine->SetState(UIStateMainMenu::UI_STATE_MAINMENU);
}

void GameMainMenuState::OnExitState() {
    DynamicObjectTrackerManager::getInstance()->clean();
    DynamicObjectTrackerManager::getInstance()->printReport();
    BaseStateDepricated::OnExitState();
}
