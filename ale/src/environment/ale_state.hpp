/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  ale_state.hpp
 *
 *  A class that stores a copy of the current ALE state. We use one to keep track
 *   of paddle resistance and in search trees.
 *  
 **************************************************************************** */

#ifndef __ALE_STATE_HPP__ 
#define __ALE_STATE_HPP__

#include "../emucore/OSystem.hxx"
#include "../emucore/Event.hxx"
#include <string>
#include "../games/RomSettings.hpp"

#define PADDLE_DELTA 23000
// MGB Values taken from Paddles.cxx (Stella 3.3) - 1400000 * [5,235] / 255
#define PADDLE_MIN 27450
// MGB - was 1290196; updated to 790196... seems to be fine for breakout and pong; 
//  avoids pong paddle going off screen
#define PADDLE_MAX 790196 
#define PADDLE_DEFAULT_VALUE (((PADDLE_MAX - PADDLE_MIN) / 2) + PADDLE_MIN)

class ALEState {
  public:
    ALEState();
    // Makes a copy of this state, also storing emulator information provided as a string
    ALEState(const ALEState &rhs, std::string serialized);

    /** Restores the environment to a previously saved state. */ 
    void load(OSystem* osystem, RomSettings* settings, std::string md5, const ALEState &rhs);

    /** Returns a "copy" of the current state, including the information necessary to restore
      *  the emulator. */
    ALEState save(OSystem* osystem, RomSettings* settings, std::string md5);

    /** Resets the system to its start state. numResetSteps 'RESET' actions are taken after the
      *  start. */
    void reset(int numResetSteps = 1);

    /** Returns true if the two states contain the same saved information */
    bool equals(ALEState &state);

    void resetPaddles(Event*);

    /** Applies paddle actions. This actually modifies the game state by updating the paddle
      *  resistances. */
    void applyActionPaddles(Event* event_obj, int player_a_action, int player_b_action);
    /** Sets the joystick events. No effect until the emulator is run forward. */
    void setActionJoysticks(Event* event_obj, int player_a_action, int player_b_action);

  protected:
    // Let StellaEnvironment access these methods: they are needed for emulation purposes
    friend class StellaEnvironment;

    /** Reset key presses */
    void resetKeys(Event* event_obj);

    /** Sets the paddle to a given position */
    void setPaddles(Event* event_obj, int left, int right);

    /** Updates the paddle position by a delta amount. */
    void updatePaddlePositions(Event* event_obj, int delta_x, int delta_y);

    /** Calculates the Paddle resistance, based on the given x val */
    int calcPaddleResistance(int x_val);
  
  private:
    int m_left_paddle;   // Current value for the left-paddle
    int m_right_paddle;  // Current value for the right-paddle

    int m_frame_number; // Current frame number

    string m_serialized_state; // The stored environment state, if this is a saved state

};

#endif // __ALE_STATE_HPP__


