// Code inspired by https://doc.photonengine.com/en-us/realtime/current/getting-started/realtime-intro
// and Photon-Windows-Sdk_v4-1-8-0\Demos\demo_basics\src\Photon_lib.cpp

#include "NetworkLogic.hpp"
#include "limits.h"
#include "Game.hpp"

#include <string>
#include <iostream>


NetworkLogic::NetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion, Game *game)
	: mLogger()
	, mState(State::INITIALIZED)
	, mGame(game)
	, mUIListener()
	, mpOutputListener(&mUIListener)
	, mLoadBalancingClient(*this, appID, appVersion, ExitGames::Photon::ConnectionProtocol::UDP, true)
	, mSendCount (0)
    , mReceiveCount(0)
{
	//mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
	mLogger.setListener(*this);
	//logger.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // this class
	ExitGames::Common::Base::setListener(this);
	//ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
}

void NetworkLogic::waitForListener(const NetworkLogic::State &originalState, const NetworkLogic::State &targetState)
{
	while (mState == originalState)
	{
		service();
		Sleep(20);
	}
	if (mState != targetState)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Arrived in wrong state ('" + state2string(mState) + L"' instead of awaited '" + state2string(targetState) + L"')");
		exit(1);
	}
}

void NetworkLogic::connect(void)
{
	if (!mLoadBalancingClient.connect())
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
	waitForListener(NetworkLogic::State::INITIALIZED, NetworkLogic::State::CONNECTED);
}

void NetworkLogic::joinOrCreateRoom(const ExitGames::Common::JString& roomName)
{
	std::wcout << "Trying to joinOrCreate room: " << roomName << std::endl;
	mLoadBalancingClient.opJoinOrCreateRoom(roomName);
	waitForListener(NetworkLogic::State::CONNECTED, NetworkLogic::State::JOINED);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void NetworkLogic::service(void)
{
	mLoadBalancingClient.service();
}

void NetworkLogic::sendPlayerChange(Menus::Action action)
{
	nByte eventCode;
	ExitGames::Common::Hashtable evData; // organize your payload data in any way you like as long as it is supported by Photons serialization


	nByte eventCodebis;
	ExitGames::Common::Hashtable evDatabis; // organize your payload data in any way you like as long as it is supported by Photons serialization

	switch (action) {
	case Menus::PlaySolo :
	case Menus::PlayMulti:
		eventCode = ChangeMode; // use distinct event codes to distinguish between different types of events (for example 'move', 'shoot', etc.)
		break;
	case Menus::Litterature:
	case Menus::Histoire:
	case Menus::Chant:
	case Menus::Musique:
	case Menus::Tragedie:
	case Menus::Comedie:
	case Menus::Danse:
	case Menus::Rhetorique:
	case Menus::Astrologie:
		eventCode = ChangeTheme; // use distinct event codes to distinguish between different types of events (for example 'move', 'shoot', etc.)
		break;
	case Menus::Vide:
		eventCode = ChangeTheme;

		eventCodebis = ChangeMode;
		evDatabis.put(static_cast<nByte>(0), static_cast<int>(action));
		mLoadBalancingClient.opRaiseEvent(true, evDatabis, eventCodebis); // true, because it is not acceptable to lose player actions
		break;
	default :
		eventCode = Error; // use distinct event codes to distinguish between different types of events (for example 'move', 'shoot', etc.)
		break;
	}

	evData.put(static_cast<nByte>(0), static_cast<int>(action));
	mLoadBalancingClient.opRaiseEvent(true, evData, eventCode); // true, because it is not acceptable to lose player actions
	++mSendCount;
}

void NetworkLogic::sendPlayerChoice(std::string choice)
{
	nByte eventCode = SendQuestions;
	ExitGames::Common::Hashtable evData; // organize your payload data in any way you like as long as it is supported by Photons serialization
	evData.put(static_cast<nByte>(0), static_cast<int>(std::stoi(choice)));
	mLoadBalancingClient.opRaiseEvent(true, evData, eventCode); // true, because it is not acceptable to lose player actions
	++mSendCount;
}

void NetworkLogic::sendPlayerScore(int score)
{
	nByte eventCode = SendScore;
	ExitGames::Common::Hashtable evData; // organize your payload data in any way you like as long as it is supported by Photons serialization
	evData.put(static_cast<nByte>(0), static_cast<int>(score));
	mLoadBalancingClient.opRaiseEvent(true, evData, eventCode); // true, because it is not acceptable to lose player actions
	++mSendCount;
}

int NetworkLogic::getNumber(void)
{
	// In photon, First player to connect receives number 1, second receives number 2, etc.
	// If player with number 1 leaves and a new player connects, this player does *not* receive
	// number 1, but the number of player which connected until now.
	return mLoadBalancingClient.getLocalPlayer().getNumber() - 1; // -1 because photon starts with player number 1
}

int NetworkLogic::getRoundTripTime(void) const
{
	return mLoadBalancingClient.getRoundTripTime();
}
int NetworkLogic::getRoundTripTimeVariance(void) const
{
	return mLoadBalancingClient.getRoundTripTimeVariance();
}

int NetworkLogic::getSendCount() const
{
	return mSendCount;
}
int NetworkLogic::getReceiveCount() const
{
	return mReceiveCount;
}

int NetworkLogic::getBytesOut(void) const
{
	return mLoadBalancingClient.getBytesOut();
}

int NetworkLogic::getBytesIn(void) const
{
	return mLoadBalancingClient.getBytesIn();
}



//////////////////////////////////////////////////////////////////
// All callbacks
//////////////////////////////////////////////////////////////////

void NetworkLogic::debugReturn(int /*debugLevel*/, const ExitGames::Common::JString& string)
{
	mpOutputListener->writeString(string);
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeString(ExitGames::Common::JString(L"received connection error ") + errorCode);
	mState = State::DISCONNECTED;
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeString(ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
	mpOutputListener->writeString(ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeString(ExitGames::Common::JString(L"received error ") + errorCode + " from server");
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	mpOutputListener->writeString(L"");
	mpOutputListener->writeString(ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");

}

void NetworkLogic::leaveRoomEventAction(int playerNr, bool isInactive)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeString(L"");
	mpOutputListener->writeString(ExitGames::Common::JString(L"player ") + playerNr + L" has left the game");
}

void NetworkLogic::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)
{
	++mReceiveCount;
	//mpOutputListener->writeString(ExitGames::Common::JString(L"customEventAction est appele pour le player ") + playerNr + L" pour l'event " + eventCode);
	ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
	switch (eventCode)
	{
	case ChangeMode:
		if (!eventContent.getValue((nByte)0)) {
			mpOutputListener->writeString(L"ERROR : Received incomplete message :-(");
			exit(1);
		}
		else {
			Menus::Action action = static_cast<Menus::Action>(((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)0)))->getDataCopy());
			mGame->opponentMode = action;
		}
		break;
		break;
	case ChangeTheme:
		if (!eventContent.getValue((nByte)0)) {
			mpOutputListener->writeString(L"ERROR : Received incomplete message :-(");
			exit(1);
		}
		else {
			Menus::Action action = static_cast<Menus::Action>(((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)0)))->getDataCopy());
			mGame->opponentTheme = action;
		}
		break;
		break;
	case SendQuestions:
		if (!eventContent.getValue((nByte)0)) {
			mpOutputListener->writeString(L"ERROR : Received incomplete message :-(");
			exit(1);
		}
		else {
			int numQuestion = static_cast<int>(((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)0)))->getDataCopy());
			(mGame->vectorChoice).push_back(std::to_string(numQuestion));
			if (mGame->currentMode == Menus::PlayMulti && mGame->opponentMode == Menus::PlayMulti && mGame->currentTheme == mGame->opponentTheme && (mGame->vectorChoice).size() == 5) {
				mGame->pageMap[Menus::QuestionGame] = std::make_shared<Questions>(mGame->vectorChoice, mGame->currentTheme, mGame->data);
				mGame->currentPage = mGame->pageMap[Menus::QuestionGame];
				dynamic_cast<Questions&>(*(mGame->currentPage)).Advance();
			}
		}
		break;
		break;
	case SendScore:
		if (!eventContent.getValue((nByte)0)) {
			mpOutputListener->writeString(L"ERROR : Received incomplete message :-(");
			exit(1);
		}
		else {
			int scoreOpp = static_cast<int>(((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)0)))->getDataCopy());
			mGame->opponentScore = scoreOpp;
		}
		break;
		break;
	default:
		mpOutputListener->writeString(ExitGames::Common::JString(L"ERROR : Received unkown eventCode: ") + eventCode);
		exit(1);
		break;
	}

}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"connected to cluster " + cluster);
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mState = State::DISCONNECTING;
		return;
	}
	mpOutputListener->writeString(L"connected to cluster " + cluster);
	mState = State::CONNECTED;
}

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeString(L"disconnected");
	mState = State::DISCONNECTED;
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeString(L"opCreateRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeString(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been created");
	mState = State::JOINED;
}

void NetworkLogic::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeString(L"opJoinOrCreateRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeString(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mState = State::JOINED;
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeString(L"opJoinRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeString(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");

	mState = State::JOINED;
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeString(L"opJoinRandomRoom() failed: " + errorString);
		mState = State::CONNECTED;
		return;
	}

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeString(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
	mState = State::JOINED;
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeString(L"opLeaveRoom() failed: " + errorString);
		mState = State::DISCONNECTING;
		return;
	}
	mState = State::LEFT;
	mpOutputListener->writeString(L"room has been successfully left");
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeString(L"joined lobby");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeString(L"left lobby");
}

ExitGames::Common::JString NetworkLogic::state2string(State state)
{
	switch (state) {
	case INITIALIZED:
		return L"INITIALIZED";
	case CONNECTING:
		return L"CONNECTING";
	case CONNECTED:
		return L"CONNECTED";
	case JOINING:
		return L"JOINING";
	case JOINED:
		return L"JOINED";
	case SENT_DATA:
		return L"SENT_DATA";
	case RECEIVED_DATA:
		return L"RECEIVED_DATA";
	case LEAVING:
		return L"LEAVING";
	case LEFT:
		return L"LEFT";
	case DISCONNECTING:
		return L"DISCONNECTING";
	case DISCONNECTED:
		return L"DISCONNECTED";
	default:
		return L"Unkonwn :-(";
	}
}


