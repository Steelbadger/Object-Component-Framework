/*
	File:	IState.hpp
	Version:	2.0
	Date:	28th March 2013; August 2012. (IGameCore)
	Authors:	Allan and Stuart Milne

	Namespace:	AllanMilne
	Exposes:	IState

	Description:
	This defines an interface (pure abstract class) for use in a strategy pattern
	in the context of the WinCore class.
	It is a revised version of the IGameCore interface used in version 1 of the framework.

	The state specific logic within the WinCore model is that it can be initialized once;
	it may then have 1 or more processing cycles of set-up, process frame loop, and clean-up.
	In terms of resource alocation, setting up and releasing:
	- Initialize() is only called once so can allocate resources used for the lifetime of the object.
	- Setup() is called when a processing cycle starts so can allocate or initiate resources for that single cycle.
	- Cleanup() is called at the end of a processing cycle so should only tidy up and/or release resources created/started by Setup()
	- resources allocated by Initialize() should be released by the destructor.

	the interface exposes the following behaviour required of a concrete strategy class:
	*	bool Initialize (HWND aWindow)
		Create any state-wide resources that wil exist for the lifetime of the object.
		the argument is the handle to the parent window; allows a concrete strategy class accesss to the window.
		Return false if any errors are detected.
		Called from WinCore::Initialize().
	*	bool Setup ()
		Set up state-wide resources at the start of a processing cycle.
		Return false if any errors are detected.
		Called from WinCore::RunApp() before the message loop.
	*	void ProcessFrame (float deltaTime)
		The logic for processing a single frame in the application state.
		the parameter is the time in seconds since this function was last called.
		Called from WinCore::RunApp() every time around the Windows message loop.
	*	void Cleanup ()
		Release and/or tidy-up state-wide resources used in a processing cycle and initiated by Setup().
		Called from WinCore::RunApp() when the Windows message loop exits.

	If returning false, Initialize() and Setup() should display specific error messages
	since this is not done in the calling WinCore context.

	This model, as well as integrating with the WinCore windows processing object,
	also allows cascaded layers of IState behaviour.

*/

#ifndef __IState_HPP__
#define __IState_HPP__

#include <Windows.h>

namespace AllanMilne {

class IState {
public:

	//--- Define virtual destructor to enable cascaded destructors to work correctly.
	//--- The concrete destructor should tidy up any resources allocated by Initialize().
	virtual ~IState(){}

	//--- Create any state-wide resources that wil exist for the lifetime of the state.
	//--- should be tidied-up in the destructor.
	virtual bool Initialize (HWND aWindow) = 0;

	//--- Set-up state-wide resources at the start of a processing cycle.
	//--- these should be tidied up by Cleanup().
	virtual bool Setup () = 0;

	//--- process a single frame.
	virtual bool ProcessFrame (const float deltaTime) = 0;

	//--- Release all state-wide resources that have been allocated by Setup().
	virtual void Cleanup () = 0;

}; // end IState interface.

} // end AllanMilne namespace.

# endif