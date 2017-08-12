/**
 *  @file   cmd_parser.cpp
 *  @brief  Utility class to parse command line argument
 *
 *  @author Christophe Ecabert
 *  @date   28/08/16
 *  Copyright (c) 2016 Christophe Ecabert. All rights reserved.
 */

#include <iostream>
#include <algorithm>

#include "facekit/core/cmd_parser.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/*
 *  @name   GetCmdOption
 *  @brief  Give the content of a given option
 *  @param  begin       Begining of the command line args
 *  @param  end         End of the command line args
 *  @param  option      Which option to look for i.e : -e
 *  @return Return the command
 */
const char* GetCmdOption(const char ** begin,
                         const char ** end,
                         const std::string & option);
/*
 *  @name   CmdOptionExists
 *  @brief  Check if the command exist inside the argument from cmd line
 *  @param  begin       Begining of the command line args
 *  @param  end         End of the command line args
 *  @param  option      Which option to look for i.e : -e
 *  @return Indicate if a command is present or not
 */
bool CmdOptionExists(const char** begin,
                     const char** end,
                     const std::string& option);

#pragma mark -
#pragma mark Initialization

/*
 *  @name CmdLineParser
 *  @fn CmdLineParser(void)
 *  @brief  Constructor
 */
CmdLineParser::CmdLineParser(void) {
  // Add help
  argument_.push_back(new Args("-h",kOptional,"Print help for this executable"));
}

/*
 *  @name CmdLineParser
 *  @fn CmdLineParser(void)
 *  @brief  Destructor
 */
CmdLineParser::~CmdLineParser(void) {
  // Clear arg
  for (int i = 0; i < argument_.size(); ++i) {
    delete argument_[i];
  }
}

#pragma mark -
#pragma mark Process

/*
 *  @name AddArgument
 *  @fn void AddArgument(const std::string& key,
                         const ArgState& state,
                         const std::string description)
 *  @brief  Add an argument needed by the system
 *  @param[in]  key         Char flag indicating for this argument
 *  @param[in]  state       Argument's state #ArgState
 *  @param[in]  desciption  Description of the arguments
 *  @return Return -1 of key already set, 0 otherwise
 */
int CmdLineParser::AddArgument(const std::string& key,
                               const ArgState& state,
                               const std::string& description) {
  int error = -1;
  std::string key_l = key[0] == '-' ? key : "-" + key;
  // Check first if key is not alread present
  if (!this->HasArgument(key_l, nullptr)) {
    // Can add if
    argument_.push_back(new CmdLineParser::Args(key_l, state, description));
    error = 0;
  }
  return error;
}

/*
 *  @name ParseCmdLine
 *  @fn int ParseCmdLine(const int argc, const char** argv)
 *  @brief  Parse command line argument
 *  @param[in]  argc  Number of element in the command line
 *  @param[in]  argv  Elements in the command line
 *  @return -1 If error, 0 otherwise
 */
int CmdLineParser::ParseCmdLine(const int argc, const char** argv) {
  int error = -1;
  // Look first for help, then for other elements
  if (FaceKit::CmdOptionExists(argv, argv + argc, "-h") ||
      (argc == 1 && argument_.size() == 1)) {
    this->PrintHelp();
  } else {
    // Check all Arguments
    std::vector<Args*>::iterator arg_it = argument_.begin();
    for (; arg_it != argument_.end(); ++arg_it) {
      // Argument present ?
      if (FaceKit::CmdOptionExists(argv, argv + argc, (*arg_it)->key)) {
        // Yes, get value
        (*arg_it)->value = FaceKit::GetCmdOption(argv,
                                              argv + argc,
                                              (*arg_it)->key);
        error = 0;
      } else {
        // Missing, optional ?
        if ((*arg_it)->state == ArgState::kOptional) {
          error = 0;
        } else {
          error = -1;
          break;
        }
      }
    }
  }
  return error;
}

/*
 *  @name HasArgument
 *  @fn bool HasArgument(const std::string& key, std::string* value) const;
 *  @brief  Check whether of not the given key is present in the command line
 *  @param[in]  key     Key to look for
 *  @param[out] value   The corresponding value for the given key if present
 *                      in the command, otherwise unset.
 *  @return True if the given key is present in the command line arguments,
 *          flase otherwise
 */
bool CmdLineParser::HasArgument(const std::string& key, std::string* value) const {
  bool has = (std::find_if(argument_.begin(),
                           argument_.end(),
                           [&](const Args* arg) ->bool {
                             bool ret = arg->key == key;
                             if (ret) {
                               *value = arg->value;
                             }
                             return ret;
                           }) != argument_.end());
  return has;
}

/*
 *  @name PrintHelp
 *  @fn void PrintHelp(void) const
 *  @brief  Print description for all arguments
 */
void CmdLineParser::PrintHelp(void) const {
  // Loop over every argument
  for (int i = 0; i < argument_.size(); ++i) {
    const Args* arg_ptr = argument_[i];
    std::cout << arg_ptr->key << " (" <<
    (arg_ptr->state == kNeeded ? "REQUIRED" : "OPTIONAL") << ") " <<
    arg_ptr->description << std::endl;
  }
}



/*
 *  @name   GetCmdOption
 *  @brief  Give the content of a given option
 *  @param  begin       Begining of the command line args
 *  @param  end         End of the command line args
 *  @param  option      Which option to look for i.e : -e
 *  @return Return the command
 */
const char* GetCmdOption(const char ** begin,
                         const char ** end,
                         const std::string & option) {
  const char ** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return 0;
}

/*
 *  @name   CmdOptionExists
 *  @brief  Check if the command exist inside the argument from cmd line
 *  @param  begin       Begining of the command line args
 *  @param  end         End of the command line args
 *  @param  option      Which option to look for i.e : -e
 *  @return Indicate if a command is present or not
 */
bool CmdOptionExists(const char** begin,
                     const char** end,
                     const std::string& option) {
  return std::find(begin, end, option) != end;
}

}  // namespace CHlib
