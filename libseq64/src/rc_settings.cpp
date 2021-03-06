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
 * \file          rc_settings.cpp
 *
 *  This module declares/defines just some of the global (gasp!) variables
 *  in this application.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-22
 * \updates       2016-10-30
 * \license       GNU GPLv2 or above
 *
 *  Note that this module also sets the legacy global variables, so that
 *  they can be used by modules that have not yet been cleaned up.
 *
 * \warning
 *      We're making the "statistics" support a configure-time option.  The
 *      run-time option will be left here, but the actual usage of it will be
 *      disabled unless configured with the --enable-statistics option.
 */

#include <stdlib.h>                     /* getenv()                     */
#include "globals.h"                    /* to support legacy variables  */

#ifdef PLATFORM_UNIX
#include <sys/types.h>                  /* for stat() and mkdir()       */
#include <sys/stat.h>
#endif

#include "file_functions.hpp"           /* make_directory()             */
#include "rc_settings.hpp"              /* seq64::rc_settings class     */
#include "settings.hpp"                 /* seq64::rc()                  */

/**
 *  Select the HOME or HOMEPATH environment variables depending on whether
 *  building for Windows or not.  Also select the appropriate directory
 *  separator for SLASH.
 */

#ifdef PLATFORM_WINDOWS
#define HOME "HOMEPATH"
#define SLASH "\\"
#else
#define HOME "HOME"
#define SLASH "/"
#endif

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace seq64
{

/**
 *  Default constructor.
 */

rc_settings::rc_settings ()
 :
    m_auto_option_save          (true),     /* legacy seq24 behavior */
    m_legacy_format             (false),
    m_lash_support              (false),
    m_allow_mod4_mode           (false),
    m_allow_snap_split          (false),
    m_allow_click_edit          (true),
    m_show_midi                 (false),
    m_priority                  (false),
    m_stats                     (false),
    m_pass_sysex                (false),
    m_with_jack_transport       (false),
    m_with_jack_master          (false),
    m_with_jack_master_cond     (false),
    m_manual_alsa_ports         (false),
    m_reveal_alsa_ports         (false),
    m_print_keys                (false),
    m_device_ignore             (false),
    m_device_ignore_num         (0),
    m_interaction_method        (e_seq24_interaction),
    m_filename                  (),
    m_jack_session_uuid         (),
    m_last_used_dir             (),
    m_config_directory          (),
    m_config_filename           (),
    m_user_filename             (),
    m_config_filename_alt       (),
    m_user_filename_alt         ()
{
    // Empty body
}

/**
 *  Copy constructor.
 *
 * \param rhs
 *      The source of the data for the copy.
 */

rc_settings::rc_settings (const rc_settings & rhs)
 :
    m_auto_option_save          (rhs.m_auto_option_save),
    m_legacy_format             (rhs.m_legacy_format),
    m_lash_support              (rhs.m_lash_support),
    m_allow_mod4_mode           (rhs.m_allow_mod4_mode),
    m_allow_snap_split          (rhs.m_allow_snap_split),
    m_allow_click_edit          (rhs.m_allow_click_edit),
    m_show_midi                 (rhs.m_show_midi),
    m_priority                  (rhs.m_priority),
    m_stats                     (rhs.m_stats),
    m_pass_sysex                (rhs.m_pass_sysex),
    m_with_jack_transport       (rhs.m_with_jack_transport),
    m_with_jack_master          (rhs.m_with_jack_master),
    m_with_jack_master_cond     (rhs.m_with_jack_master_cond),
    m_manual_alsa_ports         (rhs.m_manual_alsa_ports),
    m_reveal_alsa_ports         (rhs.m_reveal_alsa_ports),
    m_print_keys                (rhs.m_print_keys),
    m_device_ignore             (rhs.m_device_ignore),
    m_device_ignore_num         (rhs.m_device_ignore_num),
    m_interaction_method        (rhs.m_interaction_method),
    m_filename                  (rhs.m_filename),
    m_jack_session_uuid         (rhs.m_jack_session_uuid),
    m_last_used_dir             (rhs.m_last_used_dir),
    m_config_directory          (rhs.m_config_directory),
    m_config_filename           (rhs.m_config_filename),
    m_user_filename             (rhs.m_user_filename),
    m_config_filename_alt       (rhs.m_config_filename_alt),
    m_user_filename_alt         (rhs.m_user_filename_alt)
{
    // Empty body
}

/**
 *  Principal assignment operator.
 *
 * \param rhs
 *      The source of the data for the assignment.
 *
 * \return
 *      Returns a reference to the destination for use in serial assignments.
 */

rc_settings &
rc_settings::operator = (const rc_settings & rhs)
{
    if (this != &rhs)
    {
        m_auto_option_save          = rhs.m_auto_option_save;
        m_legacy_format             = rhs.m_legacy_format;
        m_lash_support              = rhs.m_lash_support;
        m_allow_mod4_mode           = rhs.m_allow_mod4_mode;
        m_allow_snap_split          = rhs.m_allow_snap_split;
        m_allow_snap_split          = rhs.m_allow_snap_split;
        m_show_midi                 = rhs.m_show_midi;
        m_priority                  = rhs.m_priority;
        m_stats                     = rhs.m_stats;
        m_pass_sysex                = rhs.m_pass_sysex;
        m_with_jack_transport       = rhs.m_with_jack_transport;
        m_with_jack_master          = rhs.m_with_jack_master;
        m_with_jack_master_cond     = rhs.m_with_jack_master_cond;
        m_manual_alsa_ports         = rhs.m_manual_alsa_ports;
        m_reveal_alsa_ports         = rhs.m_reveal_alsa_ports;
        m_print_keys                = rhs.m_print_keys;
        m_device_ignore             = rhs.m_device_ignore;
        m_device_ignore_num         = rhs.m_device_ignore_num;
        m_interaction_method        = rhs.m_interaction_method;
        m_filename                  = rhs.m_filename;
        m_jack_session_uuid         = rhs.m_jack_session_uuid;
        m_last_used_dir             = rhs.m_last_used_dir;
        m_config_directory          = rhs.m_config_directory;
        m_config_filename           = rhs.m_config_filename;
        m_user_filename             = rhs.m_user_filename;
        m_config_filename_alt       = rhs.m_config_filename_alt;
        m_user_filename_alt         = rhs.m_user_filename_alt;
    }
    return *this;
}

/**
 *  Sets the default values.
 */

void
rc_settings::set_defaults ()
{
    m_auto_option_save          = true;     /* legacy seq224 setting */
    m_legacy_format             = false;
    m_lash_support              = false;
    m_allow_mod4_mode           = false;
    m_allow_snap_split          = false;
    m_allow_click_edit          = true;
    m_show_midi                 = false;
    m_priority                  = false;
    m_stats                     = false;
    m_pass_sysex                = false;
    m_with_jack_transport       = false;
    m_with_jack_master          = false;
    m_with_jack_master_cond     = false;
    m_manual_alsa_ports         = false;
    m_reveal_alsa_ports         = false;
    m_print_keys                = false;
    m_device_ignore             = false;
    m_device_ignore_num         = 0;
    m_device_ignore_num         = e_seq24_interaction;
    m_filename.clear();
    m_jack_session_uuid.clear();
    m_last_used_dir             = "~/";
    m_config_directory          = ".config/sequencer64";
    m_config_filename           = "sequencer64.rc";
    m_user_filename             = "sequencer64.usr";
    m_config_filename_alt       = ".seq24rc";
    m_user_filename_alt         = ".seq24usr";
}

/**
 *  Provides the directory for the configuration file, and also creates the
 *  directory if necessary.
 *
 *  If the legacy format is in force, then the home directory for the
 *  configuration is (in Linux) "/home/username", and the configuration file
 *  is ".seq24rc".
 *
 *  If the new format is in force, then the home directory is (in Linux)
 *  "/home/username/.config/sequencer64", and the configuration file is
 *  "sequencer64.rc".
 *
 * \return
 *      Returns the selected home configuration directory.  If it does not
 *      exist, or could not be created, then an empty string is returned.
 */

std::string
rc_settings::home_config_directory () const
{
    std::string result;
    char * env = getenv(HOME);
    if (env != NULL)
    {
        std::string home(getenv(HOME));
        result = home + SLASH;                      /* e.g. /home/username/  */
        if (! rc().legacy_format())
        {
            result += config_directory();           /* new, longer directory */
            result += SLASH;
            bool ok = make_directory(result);
            if (! ok)
            {
                printf("? error creating [%s]\n", result.c_str());
                result.clear();
            }
        }
    }
    else
        printf("? error calling getenv(\"%s\")\n", HOME);

    return result;
}

/**
 *  Constructs the full path and file specification for the "rc" file
 *  based on whether or not the legacy Seq24 filenames are being used.
 *
 * \return
 *      If home_config_directory() returns a non-empty string, then the legacy
 *      or normal "rc" configuration file-name is appended to that result, and
 *      returned.  Otherwise, an empty string is returned.
 */

std::string
rc_settings::config_filespec () const
{
    std::string result = home_config_directory();
    if (! result.empty())
    {
        if (rc().legacy_format())
            result += config_filename_alt();
        else
            result += config_filename();
    }
    return result;
}

/**
 *  Constructs the full path and file specification for the "user" file
 *  based on whether or not the legacy Seq24 filenames are being used.
 *
 * \return
 *      If home_config_directory() returns a non-empty string, then the legacy
 *      or normal "user" configuration file-name is appended to that result,
 *      and returned.  Otherwise, an empty string is returned.
 */

std::string
rc_settings::user_filespec () const
{
    std::string result = home_config_directory();
    if (! result.empty())
    {
        if (rc().legacy_format())
            result += user_filename_alt();
        else
            result += user_filename();
    }
    return result;
}

/**
 * \setter m_device_ignore_num
 *      However, please note that this value, while set in the options
 *      processing of the main module, does not appear to be used anywhere
 *      in the code in seq24, Sequencer24, and this application.
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::device_ignore_num (int value)
{
    if (value >= 0)
        m_device_ignore_num = value;
}

/**
 * \setter m_interaction_method
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::interaction_method (interaction_method_t value)
{
    switch (value)
    {
    case e_seq24_interaction:
    case e_fruity_interaction:

        m_interaction_method = value;
        break;

    default:

        errprint("illegal interaction-method value");
        break;
    }
}

/**
 * \setter m_filename
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::filename (const std::string & value)
{
    if (! value.empty())
        m_filename = value;
}

/**
 * \setter m_jack_session_uuid
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::jack_session_uuid (const std::string & value)
{
    if (! value.empty())
        m_jack_session_uuid = value;
}

/**
 * \setter m_last_used_dir
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::last_used_dir (const std::string & value)
{
    if (! value.empty())
        m_last_used_dir = value;
}

/**
 * \setter m_config_directory
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::config_directory (const std::string & value)
{
    if (! value.empty())
        m_config_directory = value;
}

/**
 * \setter m_config_filename and m_user_filename
 *
 *      Implements the --config option to change both configuration files
 *      ("rc" and "usr") with one option.
 *
 * \param value
 *      The value to use to make the setting, if the string is not empty.
 *      If the value has an extension, it is stripped first.
 */

void
rc_settings::set_config_files (const std::string & value)
{
    if (! value.empty())
    {
        size_t ppos = value.rfind(".");
        std::string basename;
        if (ppos != std::string::npos)
            basename = value.substr(0, ppos);   /* strip after first period */
        else
            basename = value;

        config_filename(basename);
        user_filename(basename);
    }
}

/**
 * \setter m_config_filename ("rc")
 *
 * \param value
 *      The value to use to make the setting, if the string is not empty.
 *      If there is no period in the string, then ".rc" is appended to the
 *      end of the filename.
 */

void
rc_settings::config_filename (const std::string & value)
{
    if (! value.empty())
        m_config_filename = value;

    if (m_config_filename.find(".") == std::string::npos)
        m_config_filename += ".rc";
}

/**
 * \setter m_user_filename ("usr")
 *
 * \param value
 *      The value to use to make the setting, if the string is not empty.
 *      If there is no period in the string, then ".usr" is appended to the
 *      end of the filename.
 */

void
rc_settings::user_filename (const std::string & value)
{
    if (! value.empty())
        m_user_filename = value;

    if (m_user_filename.find(".") == std::string::npos)
        m_user_filename += ".usr";
}

/**
 * \setter m_config_filename_alt
 *
 * \param value
 *      The value to use to make the setting, if the string is not empty.
 */

void
rc_settings::config_filename_alt (const std::string & value)
{
    if (! value.empty())
        m_config_filename_alt = value;
}

/**
 * \setter m_user_filename_alt
 *
 * \param value
 *      The value to use to make the setting.
 */

void
rc_settings::user_filename_alt (const std::string & value)
{
    if (! value.empty())
        m_user_filename_alt = value;
}

}           // namespace seq64

/*
 * rc_settings.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

