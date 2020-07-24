#ifndef OPENSIM_COMPONENT_PATH_H_
#define OPENSIM_COMPONENT_PATH_H_
/* -------------------------------------------------------------------------- *
 *                      OpenSim: ComponentPath.h                              *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2017 Stanford University and the Authors                *
 * Author(s): Carmichael Ong                                                  *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "Path.h"

namespace OpenSim {

//==============================================================================
//                            OPENSIM COMPONENT PATH
//==============================================================================
/**
* A class for handling Paths for Components, deriving from Path. A ComponentPath
* uses a forward-slash ('/') as a separator always. It also specifies invalid
* characters in a Component name:
* - back-slash ('\\')
* - forward-slash ('/')
* - asterisk ('*')
* - plus-sign ('+')
*
* @author Carmichael Ong
*/

class OSIMCOMMON_API ComponentPath : public Path {
public:
    /**
     * Returns a normalized form of `path`. A normalized path is guaranteed to:
     *
     * - Not contain any internal or trailing relative elements (e.g. 'a/../b')
     *   - It may start with relative elements (e.g. '../a/b')
     *   - It cannot start with relative elements if the path is absolute (e.g.
     *     '/../a/b' is invalid)
     * - Not contain any invalid characters (e.g. '\\', '*')
     * - Not contain any repeated separators (e.g. 'a///b' --> 'a/b')
     * - Contain no trailing slashes, unless it resolved to root
     *   - e.g. 'a/b/c/' is normalized to 'a/b/c', but '/./a/../' is normalized
     *     to '/'
     *
     * Any attempt to step above the root of the expression with '..' will
     * result in an exception being thrown (e.g. 'a/../..' will throw).
     *
     * This method is useful for path traversal and path manipulation methods,
     * because the above guarantees ensure that (e.g.) paths can be concatenated
     * and split into individual elements using basic string manipulation
     * techniques.
     */
    static std::string normalize(std::string path);

    /**
     * Returns a pair, `(head, tail)`, where `tail` is the last component in
     * `path` and `head` is everything leading up to `tail`.
     *
     * - The `tail` will never contain a `/` (component list separator)
     * - If `path` ends in a slash, then `tail` will be empty
     * - If there is no `/` in `path` then `head` will be empty
     * - If `path` is empty, both `head` and `tail` will be empty
     * - Trailing slashes are stripped from `head`, unless it is the root
     *
     * This method does not resolve relative path elements, or check for invalid
     * path characters. See `ComponentPath::normalize` for that.
     */
    static std::pair<std::string, std::string> split(std::string path);

    // Constructors
    /// The default-constructed path is empty (an empty string).
    ComponentPath();
    
    /// Construct a ComponentPath from a string. This will clean up the
    /// path, removing and resolving "." and ".." when possible.
    ComponentPath(const std::string& path);

    /// Constructor a ComponentPath from a vector that contains all subtree
    /// node names and a bool that indicates if the path is an absolute path.
    ComponentPath(const std::vector<std::string>& pathVec, bool isAbsolute);

    // Operators
    bool operator==(const ComponentPath& other) const
    {
        return this->toString() == other.toString();
    }

    bool operator!=(const ComponentPath& other) const
    {
        return this->toString() != other.toString();
    }

    // Override virtual functions
    char getSeparator() const override { return separator; };
    std::string getInvalidChars() const override { return invalidChars; };

    /// Get an absolute path by resolving it relative to a given otherPath.
    /// If the current Path is already absolute, return the same Path.
    ComponentPath formAbsolutePath(const ComponentPath& otherPath) const;

    /// Find the relative Path between this Path and another Path (otherPath)
    /// (i.e. the Path to go FROM otherPath TO this Path). Both Paths must be 
    /// absolute.
    ComponentPath formRelativePath(const ComponentPath& otherPath) const;

    /// Return the sub-path that contains all subdirectory levels 
    /// except for the last one.
    ComponentPath getParentPath() const;

    /// Return the parent path as a string.
    std::string getParentPathString() const;

    /// Return a string of a subdirectory name at a specified level. This is
    /// 0 indexed.
    std::string getSubcomponentNameAtLevel(size_t index) const;

    /// Return a string of the name of the Component related to a 
    /// ComponentPath. This is just the last level of a ComponentPath.
    std::string getComponentName() const;

private:
    static const char separator;
    static const std::string invalidChars;
};


} // end of namespace OpenSim
#endif // OPENSIM_COMPONENT_PATH_H_
