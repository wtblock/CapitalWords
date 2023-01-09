/////////////////////////////////////////////////////////////////////////////
// Copyright � by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "KeyedCollection.h"

// an array of arguments passed into the program
vector<CString> m_arrArgs;

// a collection of unique names to be output
CKeyedCollection<CString, int> m_TotalOutput;

// collection of suffixes like Jr., Sr., III, etc.
CKeyedCollection<CString, int> m_suffixes;

// collection of titles like Mr., Mrs., Miss, Uncle, etc.
CKeyedCollection<CString, int> m_titles;

// words to ignore read from "words.txt" located with the
// executable
CKeyedCollection<CString, int> m_Ignore;

// create a concordance table output for auto-marking a Word document's index
bool m_bConcordance;

// if the input file is called "names.txt", then it is assumed to be the 
// output of this program when m_bConcordance is false, 
// i.e. First [Middle] Last is the input instead of individual words
// that is normally the input
bool m_bNames;

// handle some Unicode cases expressed in extended ASCII
const CString m_csOpenAccent = _T( "‘" );
const CString m_csCloseAccent = _T( "’" );
const CString m_csOpenQuote = _T( "“" );
// Hex 9D is undefined in extended ASCII
const CString m_csCloseQuote = _T( "�\x9d" );

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

// sentence terminators
const CString m_csNonPeriod( _T( "!?" ) );
const CString m_csTerminators( m_csNonPeriod + _T( "." ));

// word decorations
const CString m_csDecor = m_csEnclosures;
const CString m_csDecorAll = m_csEnclosures + m_csTerminators;
const CString m_csEverything = m_csDecorAll + m_csSeparators;







