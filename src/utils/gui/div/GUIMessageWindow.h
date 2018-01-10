/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2003-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
/****************************************************************************/
/// @file    GUIMessageWindow.h
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @date    Tue, 25 Nov 2003
/// @version $Id$
///
// A logging window for the gui
/****************************************************************************/
#ifndef GUIMessageWindow_h
#define GUIMessageWindow_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <fx.h>
#include <utils/gui/events/GUIEvent.h>
#include <utils/iodevices/OutputDevice.h>


// ===========================================================================
// class declarations
// ===========================================================================
class GUIGlObject;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GUIMessageWindow
 * @brief A logging window for the gui
 *
 * This class displays messages incoming to the gui from either the load or
 *  the run thread.
 *
 * The text is colored in dependence to its type (messages: green, warnings: yellow,
 *  errors: red)
 *
 * Each time a new message is passed, the window is reopened.
 */
class GUIMessageWindow : public FXText {
public:
    /** @brief Constructor
     *
     * @param[in] parent The parent window
     */
    GUIMessageWindow(FXComposite* parent);


    /// @brief Destructor
    ~GUIMessageWindow();


    virtual void setCursorPos(FXint pos, FXbool notify = FALSE);


    /** @brief Adds new text to the window
     *
     * The type of the text is determined by the first parameter
     *
     * @param[in] eType The type of the event the message was generated by
     * @param[in] msg The message
     * @see GUIEventType
     */
    void appendMsg(GUIEventType eType, const std::string& msg);


    /// @brief Adds a a separator to this log window
    void addSeparator();


    /// @brief Clears the window
    void clear();

    /// @brief register and unregister message handlers
    void registerMsgHandlers();
    void unregisterMsgHandlers();

    /// @brief switch locate links on and off
    static void enableLocateLinks(const bool val) {
        myLocateLinks = val;
    }

    /// @brief ask whether locate links is enabled
    static bool locateLinksEnabled() {
        return myLocateLinks;
    }


private:
    class MsgOutputDevice : public OutputDevice {
    public:
        MsgOutputDevice(GUIMessageWindow* msgWindow, GUIEventType type) :
            myMsgWindow(msgWindow),
            myType(type) { }

        ~MsgOutputDevice() { }

    protected:
        std::ostream& getOStream() {
            return myStream;
        }
        void postWriteHook() {
            myMsgWindow->appendMsg(myType, myStream.str());
            myStream.str("");
        }

    private:
        GUIMessageWindow* myMsgWindow;
        std::ostringstream myStream;
        GUIEventType myType;
    };

    const GUIGlObject* getActiveStringObject(const FXString& text, const FXint pos, const FXint lineS, const FXint lineE) const;

private:

    /// @brief whether messages are linked to the GUI elements
    static bool myLocateLinks;

    /// @brief The text colors used
    FXHiliteStyle* myStyles;

    /** @brief The instances of message retriever encapsulations */
    OutputDevice* myErrorRetriever, *myMessageRetriever, *myWarningRetriever;



};


#endif

/****************************************************************************/

