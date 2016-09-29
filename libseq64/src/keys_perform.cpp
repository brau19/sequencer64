/*
 *  This file is part of seq24/sequencer64.
 *
 *  seq24 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  seq24 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with seq24; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          keys_perform.cpp
 *
 *  This module defines base-class keyboard interface items, VERY TENTATIVE
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-13
 * \updates       2016-09-28
 * \license       GNU GPLv2 or above
 *
 * \change ca 2016-05-22
 *      Added pattern-edit and event-edit keys which change the pattern slot
 *      shortcut/toggle keys to bring up one of the editor dialogs.
 */

#include <stdio.h>                      /* snprintf() */

#include "gdk_basic_keys.h"
#include "keys_perform.hpp"

namespace seq64
{

/**
 *  This construction initializes a vast number of member variables, some
 *  of them public!
 */

keys_perform::keys_perform ()
 :
    m_key_show_ui_sequence_key      (true),
    m_key_show_ui_sequence_number   (false),
    m_key_events                    (),
    m_key_groups                    (),
    m_key_events_rev                (),
    m_key_groups_rev                (),
    m_key_bpm_up                    (SEQ64_apostrophe),
    m_key_bpm_dn                    (SEQ64_semicolon),
    m_key_replace                   (SEQ64_Control_L),
    m_key_queue                     (SEQ64_Control_R),
    m_key_keep_queue                (SEQ64_backslash),
    m_key_snapshot_1                (SEQ64_Alt_L),
    m_key_snapshot_2                (SEQ64_Alt_R),
    m_key_screenset_up              (SEQ64_bracketright),
    m_key_screenset_dn              (SEQ64_bracketleft),
    m_key_set_playing_screenset     (SEQ64_Home),
    m_key_group_on                  (SEQ64_igrave),
    m_key_group_off                 (SEQ64_apostrophe),       // a repeat
    m_key_group_learn               (SEQ64_Insert),
    m_key_start                     (SEQ64_space),
#ifdef SEQ64_PAUSE_SUPPORT
    m_key_pause                     (SEQ64_period),
#endif
#ifdef SEQ64_STAZED_TRANSPORT
    m_key_song_mode                 (SEQ64_F1),
    m_key_toggle_jack               (SEQ64_F2),
    m_key_menu_mode                 (SEQ64_F3),
    m_key_follow_transport          (SEQ64_F4),
    m_key_fast_forward              (SEQ64_F5),
    m_key_rewind                    (SEQ64_F6),
    m_key_pointer                   (SEQ64_F7),
#endif
#ifdef SEQ64_MAINWND_TAP_BUTTON
    m_key_tap_bpm                   (SEQ64_F9),
#endif
    m_key_pattern_edit              (SEQ64_equal),
    m_key_event_edit                (SEQ64_minus),
    m_key_stop                      (SEQ64_Escape)
{
    // Empty body
}

/**
 *  The destructor sets some running flags to false, signals this
 *  condition, then joins the input and output threads if the were
 *  launched. Finally, any active patterns/sequences are deleted.
 */

keys_perform::~keys_perform ()
{
    // Empty body
}

/**
 *  Obtains the name of the key.  In gtkmm, this is done via the
 *  gdk_keyval_name() function.  Here, in the base class, we just provide an
 *  easy-to-create string.
 *
 * \param key
 *      Provides the numeric value of the keystroke.
 *
 * \return
 *      Returns the name of the key, in the format "Key 0xkkkk".
 */

std::string
keys_perform::key_name (unsigned int key) const
{
    char temp[32];
    snprintf(temp, sizeof(temp), "Key 0x%X", key);
    return std::string(temp);
}

/**
 *  Copies fields from the transfer structure in this object.  This structure
 *  holds all of the key settings from the File / Options / Keyboard tab
 *  dialog.
 *
 * \param kpt
 *      The structure that holds the values of the keys to be used for various
 *      purposes in controlling a performance live.
 */

void
keys_perform::set_keys (const keys_perform_transfer & kpt)
{
    m_key_bpm_up                    = kpt.kpt_bpm_up;
    m_key_bpm_dn                    = kpt.kpt_bpm_dn;
    m_key_replace                   = kpt.kpt_replace;
    m_key_queue                     = kpt.kpt_queue;
    m_key_keep_queue                = kpt.kpt_keep_queue;
    m_key_snapshot_1                = kpt.kpt_snapshot_1;
    m_key_snapshot_2                = kpt.kpt_snapshot_2;
    m_key_screenset_up              = kpt.kpt_screenset_up;
    m_key_screenset_dn              = kpt.kpt_screenset_dn;
    m_key_set_playing_screenset     = kpt.kpt_set_playing_screenset;
    m_key_group_on                  = kpt.kpt_group_on;
    m_key_group_off                 = kpt.kpt_group_off;
    m_key_group_learn               = kpt.kpt_group_learn;
    m_key_start                     = kpt.kpt_start;
#ifdef SEQ64_PAUSE_SUPPORT
    m_key_pause                     = kpt.kpt_pause;
#endif
#ifdef SEQ64_STAZED_TRANSPORT
    m_key_song_mode                 = kpt.kpt_song_mode;
    m_key_toggle_jack               = kpt.kpt_toggle_jack;
    m_key_menu_mode                 = kpt.kpt_menu_mode;
    m_key_follow_transport          = kpt.kpt_follow_transport;
    m_key_fast_forward              = kpt.kpt_fast_forward;
    m_key_rewind                    = kpt.kpt_rewind;
    m_key_pointer                   = kpt.kpt_pointer;
#endif
#ifdef SEQ64_MAINWND_TAP_BUTTON
    m_key_tap_bpm                   = kpt.kpt_tap_bpm;
#endif
    m_key_pattern_edit              = kpt.kpt_pattern_edit;
    m_key_event_edit                = kpt.kpt_event_edit;
    m_key_stop                      = kpt.kpt_stop;
    m_key_show_ui_sequence_key      = kpt.kpt_show_ui_sequence_key;
    m_key_show_ui_sequence_number   = kpt.kpt_show_ui_sequence_number;
}

/**
 *  Copies fields from this object into the transfer structure.
 *
 * \param kpt
 *      The structure that holds the values of the keys to be used for various
 *      purposes in controlling a performance live.
 */

void
keys_perform::get_keys (keys_perform_transfer & kpt)
{
     kpt.kpt_bpm_up                 = m_key_bpm_up;
     kpt.kpt_bpm_dn                 = m_key_bpm_dn;
     kpt.kpt_replace                = m_key_replace;
     kpt.kpt_queue                  = m_key_queue;
     kpt.kpt_keep_queue             = m_key_keep_queue;
     kpt.kpt_snapshot_1             = m_key_snapshot_1;
     kpt.kpt_snapshot_2             = m_key_snapshot_2;
     kpt.kpt_screenset_up           = m_key_screenset_up;
     kpt.kpt_screenset_dn           = m_key_screenset_dn;
     kpt.kpt_set_playing_screenset  = m_key_set_playing_screenset;
     kpt.kpt_group_on               = m_key_group_on;
     kpt.kpt_group_off              = m_key_group_off;
     kpt.kpt_group_learn            = m_key_group_learn;
     kpt.kpt_start                  = m_key_start;
#ifdef SEQ64_PAUSE_SUPPORT
     kpt.kpt_pause                  = m_key_pause;
#endif
#ifdef SEQ64_STAZED_TRANSPORT
    kpt.kpt_song_mode               = m_key_song_mode;
    kpt.kpt_toggle_jack             = m_key_toggle_jack;
    kpt.kpt_menu_mode               = m_key_menu_mode;
    kpt.kpt_follow_transport        = m_key_follow_transport;
    kpt.kpt_fast_forward            = m_key_fast_forward;
    kpt.kpt_rewind                  = m_key_rewind;
    kpt.kpt_pointer                 = m_key_pointer;
#endif
#ifdef SEQ64_MAINWND_TAP_BUTTON
    kpt.kpt_tap_bpm                 = m_key_tap_bpm;
#endif
    kpt.kpt_pattern_edit            = m_key_pattern_edit;
    kpt.kpt_event_edit              = m_key_event_edit;
    kpt.kpt_stop                    = m_key_stop;
    kpt.kpt_show_ui_sequence_key    = m_key_show_ui_sequence_key;
    kpt.kpt_show_ui_sequence_number = m_key_show_ui_sequence_number;
}

/**
 *  At construction time, this function sets up one keycode and one event
 *  slot.  It is called 32 times, corresponding the pattern/sequence slots in
 *  the Patterns window.
 *
 * \param keycode
 *      The key to be assigned.
 *
 * \param sequence_slot
 *      The perform event slot into which the keycode will be assigned.
 */

void
keys_perform::set_key_event (unsigned int keycode, long sequence_slot)
{
    SlotMap::iterator it1 = m_key_events.find(keycode);
    if (it1 != m_key_events.end())
    {
        RevSlotMap::iterator i = m_key_events_rev.find(it1->second);
        if (i != m_key_events_rev.end())
            m_key_events_rev.erase(i);          /* unhook previous binding  */

        m_key_events.erase(it1);                /* unhook the reverse one   */
    }

    RevSlotMap::iterator it2 = m_key_events_rev.find(sequence_slot);
    if (it2 != m_key_events_rev.end())
    {
        SlotMap::iterator i = m_key_events.find(it2->second);
        if (i != m_key_events.end())
            m_key_events.erase(i);              /* unhook previous binding  */

        m_key_events_rev.erase(it2);            /* unhook the reverse one   */
    }
    m_key_events[keycode] = sequence_slot;      /* set the new binding      */
    m_key_events_rev[sequence_slot] = keycode;
}

/**
 *  At construction time, this function sets up one keycode and one group
 *  slot.  It is called 32 times, corresponding the pattern/sequence slots in
 *  the Patterns window.
 *
 * \param keycode
 *      The key to be assigned.
 *
 * \param group_slot
 *      The perform group slot into which the keycode will be assigned.
 */

void
keys_perform::set_key_group (unsigned int keycode, long group_slot)
{
    SlotMap::iterator it1 = m_key_groups.find(keycode);
    if (it1 != m_key_groups.end())
    {
        RevSlotMap::iterator i = m_key_groups_rev.find(it1->second);
        if (i != m_key_groups_rev.end())
            m_key_groups_rev.erase(i);          /* unhook previous binding  */

        m_key_groups.erase(it1);
    }

    RevSlotMap::iterator it2 = m_key_groups_rev.find(group_slot);
    if (it2 != m_key_groups_rev.end())
    {
        SlotMap::iterator i = m_key_groups.find(it2->second);
        if (i != m_key_groups.end())
            m_key_groups.erase(i);              /* unhook previous binding  */

        m_key_groups_rev.erase(it2);
    }
    m_key_groups[keycode] = group_slot;         /* set the new binding      */
    m_key_groups_rev[group_slot] = keycode;
}

/*
 * Functions for the key-transfer structure.
 */

/**
 *  For the case in which the "rc" file is missing or corrupt, this function
 *  makes sure that each control key has a reasonable value.  Otherwise, random
 *  values, unchecked, can cause the application to crash.
 *
 *  Any field that is 0 or greater than 65536 is fixed.  Not perfect, but
 *  better than allowing random values to be used.
 *
 * \param k
 *      The structure to be validated and normalized.
 */

void
keyval_normalize (keys_perform_transfer & k)
{
    if (invalid_key(k.kpt_bpm_up))
        k.kpt_bpm_up = SEQ64_apostrophe;                /* '        */

    if (invalid_key(k.kpt_bpm_dn))
        k.kpt_bpm_dn = SEQ64_semicolon;                 /* ;        */

    if (invalid_key(k.kpt_replace))
        k.kpt_replace = SEQ64_Control_L;                /* Ctrl-L   */

    if (invalid_key(k.kpt_queue))
        k.kpt_queue = SEQ64_KP_Divide;                  /* Keypad-/ */

    if (invalid_key(k.kpt_keep_queue))
        k.kpt_keep_queue = SEQ64_backslash;             /* \        */

    if (invalid_key(k.kpt_snapshot_1))
        k.kpt_snapshot_1 = SEQ64_Alt_L;                 /* Alt-L    */

    if (invalid_key(k.kpt_snapshot_2))
        k.kpt_snapshot_2 = SEQ64_Alt_R;                 /* Alt-R    */

    if (invalid_key(k.kpt_screenset_up))
        k.kpt_screenset_up = SEQ64_bracketright;        /* ]        */

    if (invalid_key(k.kpt_screenset_dn))
        k.kpt_screenset_dn = SEQ64_bracketright;        /* [        */

    if (invalid_key(k.kpt_set_playing_screenset))
        k.kpt_set_playing_screenset = SEQ64_Home;       /* Home     */

    if (invalid_key(k.kpt_group_on))
        k.kpt_group_on = SEQ64_igrave;                  /* `        */

    /*
     * TODO:  fix this redundancy
     */

    if (invalid_key(k.kpt_group_off))
        k.kpt_group_off = SEQ64_apostrophe;             /* bpm up!! */

    if (invalid_key(k.kpt_group_learn))
        k.kpt_group_learn = SEQ64_Insert;               /* Insert   */

    if (invalid_key(k.kpt_start))
        k.kpt_start = SEQ64_space;                      /* ' '      */

#ifdef SEQ64_PAUSE_SUPPORT
    if (invalid_key(k.kpt_pause))
        k.kpt_pause = SEQ64_period;                     /* .        */
#endif

#ifdef SEQ64_STAZED_TRANSPORT
    if (invalid_key(k.kpt_song_mode))
        k.kpt_song_mode = SEQ64_F1;

    if (invalid_key(k.kpt_toggle_jack))
        k.kpt_toggle_jack = SEQ64_F2;

    if (invalid_key(k.kpt_menu_mode))
        k.kpt_menu_mode = SEQ64_F3;

    if (invalid_key(k.kpt_follow_transport))
        k.kpt_follow_transport = SEQ64_F4;

    if (invalid_key(k.kpt_fast_forward))
        k.kpt_fast_forward = SEQ64_F5;

    if (invalid_key(k.kpt_rewind))
        k.kpt_rewind = SEQ64_F6;

    if (invalid_key(k.kpt_pointer))
        k.kpt_pointer = SEQ64_F7;
#endif

#ifdef SEQ64_MAINWND_TAP_BUTTON
    if (invalid_key(k.kpt_tap_bpm))
        k.kpt_tap_bpm = SEQ64_F9;
#endif

    if (invalid_key(k.kpt_pattern_edit))
        k.kpt_pattern_edit = SEQ64_equal;               /* =        */

    if (invalid_key(k.kpt_event_edit))
        k.kpt_event_edit = SEQ64_minus;                 /* -        */

    if (invalid_key(k.kpt_stop))
        k.kpt_stop = SEQ64_Escape;                      /* Escape   */
}

}           // namespace seq64

/*
 * keys_perform.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

