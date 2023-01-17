/////////////////////////////////////////////////////////////////////////////
// Copyright © by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "KeyedCollection.h"

// an array of arguments passed into the program
vector<CString> m_arrArgs;

// number of command line arguments
size_t m_nArgs = 0;

// a collection of unique names to be output
CKeyedCollection<CString, int> m_TotalOutput;

// collection of suffixes like Jr., Sr., III, etc.
CKeyedCollection<CString, int> m_suffixes;

// collection of titles like Mr., Mrs., Miss, Uncle, etc.
CKeyedCollection<CString, int> m_titles;

// words to ignore read from "dictionary.txt" located 
// with the executable
CKeyedCollection<CString, int> m_Dictionary;

// names of people
CKeyedCollection<CString, int> m_People;

// words to ignore read from input file because they are
// both uppercase and in the dictionary
CKeyedCollection<CString, int> m_Ignore;

// names to exclude from the output because they are
// in the "exclude.txt" in the same folder as the input
// file.
CKeyedCollection<CString, int> m_Exclude;

// create a concordance table output for auto-marking a Word 
// document's index
bool m_bConcordance;

// create an uppercase file representing all uppercase words
// found in the original input file.
bool m_bUppercase;

// similar to uppercase, but words are rejected when the
// lowercase version of the word is found in the dictionary
bool m_bPeople;

// create an lowercase file representing all lowercase words
// found in the original input file.
bool m_bLowercase;

// the same as lowercase, but the collection is first initialized
// with the dictionary words which provides a method of building
// the dictionary
bool m_bDictionary;

// this stream can be redirected from the command line to allow the 
// output you are interested in to be captured into another file
// (Ex. > out_file.csv)
CStdioFile m_fOut( stdout );

// this stream is not redirected; it only shows up on the console and
// will not affect the output file that is being redirected to
CStdioFile m_fErr( stderr );

// console messages
CString m_csMessage;

// data file
CStdioFile m_file;

// if the input file is called "names.txt", then it is assumed to be the 
// output of this program when m_bConcordance is false, 
// i.e. First [Middle] Last is the input instead of individual words
// that is normally the input
bool m_bNames;

// handle some Unicode cases expressed in extended ASCII
const CString m_csOpenAccent = _T( "â€˜" );
const CString m_csCloseAccent = _T( "â€™" );
const CString m_csOpenQuote = _T( "â€œ" );
// Hex 9D is undefined in extended ASCII
const CString m_csCloseQuote = _T( "â€\x9d" );

// handle some dumb cases
const CString m_csDumbQuote = _T( "\"" );
const CString m_csDumbAccent( _T( "'" ));
const CString m_csParenthesis( _T( "()" ));

// enclosures
const CString m_csEnclosures = 
	m_csOpenAccent + m_csCloseAccent + 
	m_csOpenQuote + m_csCloseQuote + 
	m_csDumbQuote + m_csDumbAccent + 
	m_csParenthesis;

// name separators
const CString m_csSeparators = _T( ",;:" );

// symbols
const CString m_csSymbols = _T( " `~^[]{}=-+*/&%$#@<>|" );

// numbers
const CString m_csNumbers = _T( "0123456789" );

// sentence terminators
const CString m_csNonPeriod( _T( "!?" ) );
const CString m_csTerminators( m_csNonPeriod + _T( "." ));

// word decorations
const CString m_csDecor = m_csEnclosures;
const CString m_csDecorAll = m_csEnclosures + m_csTerminators;
const CString m_csEverything = m_csDecorAll + m_csSeparators + 
	+ m_csSymbols + m_csNumbers;







