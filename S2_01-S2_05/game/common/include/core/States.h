#pragma once

#include <common.h>

class StateBase {
public:
	StateBase(const char* name) {
		this->name = name;
	}

private:
	const char* name;
};

template <class TOwner>
class State : public StateBase {
public:
	typedef void (TOwner::* funcPtr)();

	State(const char* name, funcPtr begin, funcPtr execute, funcPtr end) : StateBase(name) {
		this->begin = new funcPtr;
		this->execute = new funcPtr;
		this->end = new funcPtr;

		*this->begin = begin;
		*this->execute = execute;
		*this->end = end;
	}

	~State() {
		delete this->begin;
		delete this->execute;
		delete this->end;
	}

	funcPtr* begin;
	funcPtr* execute;
	funcPtr* end;
};

template <class TOwner>
class StateManager {
public:
	State<TOwner>* currentState = 0;
	State<TOwner>* nextState = 0;
	u32 stateFrameCount = 0;
	TOwner* owner = 0;

	StateManager(TOwner* owner) {
		this->owner = owner;
	}

	//template <typename T>
	void setState(StateBase* state) {
		nextState = (State<TOwner>*)state;
	}

	State<TOwner>* getCurrentState() { return currentState; }

	void execute() {
		if (!currentState) {
			if (nextState) {
				currentState = nextState;
				nextState = 0;
			}
			else return;
		}

		if (nextState) {
			(owner->*(*currentState->end))();
			currentState = nextState;
			nextState = 0;
			stateFrameCount = 0;
		}

		if (stateFrameCount == 0)
			(owner->*(*currentState->begin))();

		if(nextState == 0)
			(owner->*(*currentState->execute))();

		stateFrameCount++;
	}
};

#define DECLARE_STATE(NAME) \
	static cState StateID_##NAME; \
	void beginState_##NAME(); \
	void executeState_##NAME(); \
	void endState_##NAME();

#define DECLARE_STATE_VIRTUAL(NAME) \
	static cState StateID_##NAME; \
	virtual void beginState_##NAME(); \
	virtual void executeState_##NAME(); \
	virtual void endState_##NAME();

#define CREATE_STATE(CLASS, NAME) \
	CLASS::cState CLASS::StateID_##NAME \
		(#CLASS "::StateID_" #NAME, \
		&CLASS::beginState_##NAME, \
		&CLASS::executeState_##NAME, \
		&CLASS::endState_##NAME);

#define USING_STATES(CLASS) \
	typedef State<CLASS> cState;
