/*
 * AttackPatterns.hpp
 *
 * Copyright (C) 2013-2014 University of California Santa Barbara.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335,
 * USA.
 *
 * Authors: Abdulbaki Aydin, Muath Alkhalaf, Thomas Barber
 */

#ifndef ATTACKPATTERNS_HPP_
#define ATTACKPATTERNS_HPP_

#include "StrangerAutomaton.hpp"
#include "AttackContext.hpp"

class AttackPatterns {

public:

    static StrangerAutomaton* getAttackPatternForContext(AttackContext context);

    static StrangerAutomaton* getLiteralPattern();
    static StrangerAutomaton* lessThanPattern();

    /******************************************************************************************
     * Attack Patterns
     *****************************************************************************************/

    // Allowed characters in innerHTML, excludes ">", "<", "'", """, "\"
    // & is allowed only if escaped
    static StrangerAutomaton* getHtmlPattern();

    // Allowed characters in HTML attribute, excludes all non alphanumeric chars, except & escaped 
    static StrangerAutomaton* getHtmlAttributePattern();

    // Only allow alphanumeric, "," "." "_" and whitespace, all others must be JS escaped
    static StrangerAutomaton* getJavascriptPattern();

    // Only allow alphanumeric, "-", "_", "." "~" and URL escaped characters
    static StrangerAutomaton* getUrlPattern();

    // A sample html payload
    static StrangerAutomaton* getHtmlPayload();

    /******************************************************************************************
     * Common Sanitizer Patterns for comparison
     *****************************************************************************************/

    // Only characters not excluded by getHtmlPattern()
    static StrangerAutomaton* getHtmlEscaped();

    // Only characters not excluded by getHtmlAttrPattern()
    static StrangerAutomaton* getHtmlAttrEscaped();

    // Only characters not excluded by getJavascriptPattern()
    static StrangerAutomaton* getJavascriptEscaped();

    // Only characters not excluded by getUrlPattern()
    static StrangerAutomaton* getUrlEscaped();

    // No HTML characters allowed (not even escaped &) <>'\"&/
    static StrangerAutomaton* getHtmlRemoved();

    // No HTML characters allowed, except slash <>'\"&
    static StrangerAutomaton* getHtmlRemovedNoSlash();

    // Characters allowed by UtiComponentEncode
    static StrangerAutomaton* getUrlComponentEncoded();

    // HTML escaped <>&
    static StrangerAutomaton* getEncodeHtmlNoQuotes();

    // HTML escaped <>&"
    static StrangerAutomaton* getEncodeHtmlCompat();

    // HTML escaped <>&"'
    static StrangerAutomaton* getEncodeHtmlQuotes();

    // HTML escaped <>&"'/
    static StrangerAutomaton* getEncodeHtmlSlash();

    static StrangerAutomaton* getUndesiredSQLTest();
    static StrangerAutomaton* getUndesiredMFETest();

private:
    static StrangerAutomaton* getAllowedFromRegEx(const std::string& regex);
    static StrangerAutomaton* getAttackPatternFromAllowedRegEx(const std::string& regex);

    static std::string m_htmlEscapedRegExp;
    static std::string m_htmlAttrEscapedRegExp;
    static std::string m_javascriptEscapedRegExp;
    static std::string m_urlEscapedRegExp;
    static std::string m_htmlPayload;
    static std::string m_htmlRemovedRegExp;
    static std::string m_htmlRemovedNoSlashRegExp;
};


#endif /* ATTACKPATTERNS_HPP_ */
