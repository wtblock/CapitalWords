/////////////////////////////////////////////////////////////////////////////
// Copyright © by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHelper.h"
#include "CapitalWords.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
CWinApp theApp;

/////////////////////////////////////////////////////////////////////////////
// return the number of given characters in the output vector
int CountCharacters( TCHAR input, vector<CString>& Output )
{
	int value = 0;

	for ( auto& node : Output )
	{
		int nStart = 0;
		do
		{
			const int nPos = node.Find( input, nStart );
			if ( nPos == -1 )
			{
				break;
			}

			value++;
			nStart = nPos + 1;
		}
		while ( true );
	}

	return value;
} // CountCharacters

/////////////////////////////////////////////////////////////////////////////
// is the output enclosed in parenthesis, the first character and last 
// character are matching open and close parenthesis where:
//	(xxx yyy) is true
//	(xxx) (yyy) is false
bool EnclosedParenthesis( vector<CString>& Output )
{
	bool value = false;
	const size_t nWords = Output.size();
	if ( nWords > 0 )
	{
		const int nOpen = CountCharacters( '(', Output );
		const int nClose = CountCharacters( ')', Output );
		if ( nOpen == 1 && nClose == 1 )
		{
			// is first character of first word an open parenthesis?
			CString csFirst = Output[ 0 ];
			if ( csFirst.Left( 1 ) == _T( "(" ) )
			{
				// is the last character of the last word a 
				// close parenthesis?
				CString csLast = Output[ nWords - 1 ];
				
				// handle case where there is a comma at the end
				// before the close parenthesis
				csLast.TrimRight( _T( ","));

				if ( csLast.Right( 1 ) == _T( ")" ))
				{
					value = true;
				}
			}
		}
	}

	return value;
} // EnclosedParenthesis

/////////////////////////////////////////////////////////////////////////////
// return the number of given substring in the output vector
int CountSubstrings( CString input, vector<CString>& Output )
{
	int value = 0;

	for ( auto& node : Output )
	{
		int nStart = 0;
		do
		{
			const int nPos = node.Find( input, nStart );
			if ( nPos == -1 )
			{
				break;
			}

			value++;
			nStart = nPos + 1;
		}
		while ( true );
	}

	return value;
} // CountSubstrings

/////////////////////////////////////////////////////////////////////////////
// is the output enclosed in quotes, the first character and last 
// character are matching open and close quotes where:
//	"xxx yyy" is true
//	"xxx" "yyy" is false
// The "quotes" being tested are the Unicode three character strings
bool EnclosedQuotes( vector<CString>& Output )
{
	bool value = false;
	const size_t nWords = Output.size();
	if ( nWords > 0 )
	{
		const int nOpen = CountSubstrings( m_csOpenQuote, Output );
		const int nClose = CountSubstrings( m_csCloseQuote, Output );

		if ( nOpen == 1 && nClose == 1 )
		{
			// is first character of first word an open quote?
			CString csFirst = Output[ 0 ];
			if ( csFirst.Left( 3 ) == m_csOpenQuote )
			{
				// is the last character of the last word a 
				// close parenthesis?
				CString csLast = Output[ nWords - 1 ];
				
				// handle case where there is a comma at the end
				// before the close parenthesis
				csLast.TrimRight( _T( ","));

				if ( csLast.Right( 3 ) == m_csCloseQuote )
				{
					value = true;
				}
			}
		}
	}

	return value;
} // EnclosedQuotes

/////////////////////////////////////////////////////////////////////////////
// an orphan parenthesis is a leading open parenthesis without a close
// or a trailing close parenthesis without an open
bool OrphanParenthesis( vector<CString>& Output )
{
	bool value = false;
	const size_t nWords = Output.size();
	if ( nWords > 0 )
	{
		const int nOpen = CountCharacters( '(', Output );
		const int nClose = CountCharacters( ')', Output );
		if ( nOpen == 1 && nClose == 0 )
		{
			// is first character of first word an open parenthesis?
			const CString csFirst = Output[ 0 ];
			TCHAR cLeft = csFirst[ 0 ];
			if ( cLeft == '(' )
			{
				value = true;
			}
		} 
		else if ( nOpen == 0 && nClose == 1 )
		{
			// is the last character of the last word a 
			// close parenthesis?
			CString csLast = Output[ nWords - 1 ];

			// handle the case where there is a trailing comma
			csLast.TrimRight( _T( ",." ));

			const int nLen = csLast.GetLength();
			TCHAR cRight = csLast[ nLen - 1 ];
			if ( cRight == ')' )
			{
				value = true;
			}
		}
	}

	return value;
} // OrphanParenthesis

/////////////////////////////////////////////////////////////////////////////
// an orphan quote is a leading open quote without a close
// or a trailing close quote without an open
bool OrphanQuotes( vector<CString>& Output )
{
	bool value = false;
	const size_t nWords = Output.size();
	if ( nWords > 0 )
	{
		const int nOpen = CountSubstrings( m_csOpenQuote, Output );
		const int nClose = CountSubstrings( m_csCloseQuote, Output );
		if ( nOpen == 1 && nClose == 0 )
		{
			// is first character of first word an open quote?
			const CString csFirst = Output[ 0 ];

			// a Unicode quote is three characters long
			const CString csLeft = csFirst.Left( 3 );
			if ( csLeft == m_csOpenQuote )
			{
				value = true;
			}
		} 
		else if ( nOpen == 0 && nClose == 1 )
		{
			// is the last character of the last word a 
			// close parenthesis?
			CString csLast = Output[ nWords - 1 ];

			// handle the case where there is trailing punctuation
			csLast.TrimRight( _T( ";,." ));

			// a Unicode quote is three characters long
			const CString csRight = csLast.Right( 3 );
			if ( csRight == m_csCloseQuote )
			{
				value = true;
			}
		}
	}

	return value;
} // OrphanQuotes

/////////////////////////////////////////////////////////////////////////////
// for the case where the is an unclosed parenthesis:
//		word1, word2, (word3, word4, word5...
// The output buffer will be replaced with the words prior to the 
// open parenthesis
void HandleUnclosedParenthesis( vector<CString>& Output )
{
	int nOpen = 0;
	int nClose = 0;
	vector<CString> buffer;

	int nWord = 0;
	for ( auto& node : Output )
	{
		const int nLen = Output[ nWord ].GetLength();
		if ( nLen != 0 )
		{
			if ( Output[ nWord ][ 0 ] == '(' )
			{
				nOpen++;
			}
			if ( Output[ nWord ][ nLen - 1 ] == ')' )
			{
				nClose++;
			}
		}

		nWord++;
	}

	// test for an unclosed parenthesis pair
	if ( nOpen == 1 && nClose == 0 )
	{
		// copy words from output to buffer until
		// the open parenthesis is found
		nWord = 0;
		for ( auto& node : Output )
		{
			if ( nWord == 0 )
			{
				buffer.push_back( Output[ nWord++ ] );
				continue;
			}

			if ( Output[ nWord ][ 0 ] == '(' )
			{
				break;
			}

			buffer.push_back( Output[ nWord++ ] );
		}

		// return the modified output
		Output = buffer;
	}

} // HandleUnclosedParenthesis

/////////////////////////////////////////////////////////////////////////////
// for the case where there is an unclosed quote:
//		word1, word2, "word3, word4, word5...
// The output buffer will be replaced with the words prior to the 
// open quote
void HandleUnclosedQuotes( vector<CString>& Output )
{
	int nOpen = 0;
	int nClose = 0;
	vector<CString> buffer;

	int nWord = 0;
	for ( auto& node : Output )
	{
		const int nLen = Output[ nWord ].GetLength();
		if ( nLen != 0 )
		{
			if ( Output[ nWord ].Left( 3 ) == m_csOpenQuote )
			{
				nOpen++;
			}
			if ( Output[ nWord ].Right( 3 ) == m_csCloseQuote )
			{
				nClose++;
			}
		}

		nWord++;
	}

	// test for an unclosed quote pair
	if ( nOpen == 1 && nClose == 0 )
	{
		// copy words from output to buffer until
		// the open quote is found
		nWord = 0;
		for ( auto& node : Output )
		{
			if ( nWord == 0 )
			{
				buffer.push_back( Output[ nWord++ ] );
				continue;
			}

			if ( Output[ nWord ].Left( 3 ) == m_csOpenQuote )
			{
				break;
			}

			buffer.push_back( Output[ nWord++ ] );
		}

		// return the modified output
		Output = buffer;
	}

} // HandleUnclosedQuotes

/////////////////////////////////////////////////////////////////////////////
// if the whole output vector is enclosed in matching parenthesis, strip
// the parenthesis from the output or if there is an unmatched open or
// close parenthesis at the beginning or end, remove it.
void StripParenthesis( vector<CString>& Output )
{
	const bool bEnclosed = EnclosedParenthesis( Output );
	const bool bOrphan = OrphanParenthesis( Output );
	if ( bEnclosed || bOrphan )
	{
		const size_t nWords = Output.size();
		Output[ 0 ].Remove( '(' );

		// handle case where there is a comma at the end
		// before the close parenthesis
		Output[ nWords - 1 ].TrimRight( _T( ",." ));
		Output[ nWords - 1 ].Remove( ')' );
	}

} // StripParenthesis

/////////////////////////////////////////////////////////////////////////////
// if the whole output vector is enclosed in matching quotes, strip
// the quotes from the output or if there is an unmatched open or
// close parenthesis at the beginning or end, remove it.
void StripQuotes( vector<CString>& Output )
{
	const bool bEnclosed = EnclosedQuotes( Output );
	const bool bOrphan = OrphanQuotes( Output );
	if ( bEnclosed || bOrphan )
	{
		const size_t nWords = Output.size();
		Output[ 0 ].TrimLeft( m_csOpenQuote );

		// handle case where there is punctuation at the end
		// adjacent to the close quote
		Output[ nWords - 1 ].TrimRight( m_csCloseQuote + _T( ";,." ));
	}

} // StripQuotes

/////////////////////////////////////////////////////////////////////////////
// enclosures are open and close parenthesis and open and close quotes
// they can be safely stripped if they are at the opening or closing of
// the output
void StripEnclosures( vector<CString>& Output )
{
	StripQuotes( Output );
	StripParenthesis( Output );

	// calling a second time in case removing parenthesis exposed the quotes
	StripQuotes( Output );

} // StripEnclosures

/////////////////////////////////////////////////////////////////////////////
// strip a period (also !, *, and ?) from the last word if not a standard suffix 
// like O.P., Jr., and Sr.
void StripPeriod( vector<CString>& Output )
{
	const size_t nWords = Output.size();
	if ( nWords > 0 )
	{
		CString& csLast = Output[ nWords - 1 ];
		csLast.TrimRight( _T( "\"" ));
		const bool bTrailingPeriod = csLast.Right( 1 ) == _T( "." );
		if ( bTrailingPeriod )
		{
			if 
			( 
				csLast != _T( "O.P." ) &&
				csLast != _T( "Sr." ) &&
				csLast != _T( "Jr." )
			)
			{
				csLast.TrimRight( _T( ".!?*" ) );
			}
		}
	}

} // StripPeriod

/////////////////////////////////////////////////////////////////////////////
// does the given string begin with an uppercase character after excluding
// decorations (commas, parenthesis, quotes, and accents)
bool IsUppercase( CString input )
{
	bool value = false;

	// remove potential trailing comma and close parenthesis
	input.TrimRight( m_csCloseQuote + _T( ",)" ));

	// remove potential open parenthesis
	input.TrimLeft( m_csOpenQuote + _T( "(" ));

	// the length must be greater than zero at this point
	const int nLen = input.GetLength();
	if ( nLen > 0 )
	{
		// leading character of the input
		const TCHAR cFirst = input[ 0 ];

		value = cFirst >= 'A' && cFirst <= 'Z';
	}

	return value;
} // IsUppercase

/////////////////////////////////////////////////////////////////////////////
// is the given string an initial like W. or T. (two characters with the
// second one being a period)
bool IsAnInitial( CString input )
{
	bool value = false;

	// remove potential trailing comma and close parenthesis
	input.TrimRight( m_csCloseQuote + _T( ",)" ));

	// remove potential open parenthesis
	input.TrimLeft( m_csOpenQuote + _T( "(" ));

	const int nLen = input.GetLength();
	if ( nLen == 2 )
	{
		// second character is a period
		if ( input[ 1 ] == '.' )
		{
			// first character is uppercase
			if ( input[ 0 ] >= 'A' && input[ 0 ] <= 'Z' )
			{
				value = true;
			}
		}
	}

	return value;
} // IsAnInitial

/////////////////////////////////////////////////////////////////////////////
// only initials is not valid
bool InvalidInitials( vector<CString>& Output )
{
	bool value = true;
	for ( auto& node : Output )
	{
		if ( !IsAnInitial( node ) )
		{
			value = false;
			break;
		}
	}

	// test for a trailing initial
	if ( value == false )
	{
		const size_t nWords = Output.size();
		if ( nWords > 0 )
		{
			// trailing initial on the last word?
			if ( IsAnInitial( Output[ nWords - 1 ] ) )
			{
				value = true;
			}
		}
	}

	return value;
} // InvalidInitials

/////////////////////////////////////////////////////////////////////////////
// create a collection of name titles (prefixes)
void BuildTitleData()
{
	CString arrValues[] =
	{
		_T( "Uncle" ),
		_T( "(Uncle" ),
		_T( "(Uncle)" ),
		_T( "Uncle)" ),

		_T( "Captain" ),
		_T( "(Captain" ),
		_T( "(Captain)" ),
		_T( "Captain)" ),

		_T( "Gov." ),
		_T( "(Gov." ),
		_T( "(Gov.)" ),
		_T( "Gov.)" ),

		_T( "Mrs." ),
		_T( "(Mrs." ),
		_T( "(Mrs.)" ),
		_T( "Mrs.)" ),

		_T( "Mr." ),
		_T( "(Mr." ),
		_T( "(Mr.)" ),
		_T( "Mr.)" ),

		_T( "St." ),
		_T( "(St." ),
		_T( "(St.)" ),
		_T( "St.)" ),

		_T( "Dr." ),
		_T( "(Dr." ),
		_T( "(Dr.)" ),
		_T( "Dr.)" ),

		_T( "Rev." ),
		_T( "(Rev." ),
		_T( "(Rev.)" ),
		_T( "Rev.)" ),

		_T( "Fr." ),
		_T( "(Fr." ),
		_T( "(Fr.)" ),
		_T( "Fr.)" ),

		_T( "Capt." ),
		_T( "(Capt." ),
		_T( "(Capt.)" ),
		_T( "Capt.)" ),

		_T( "Drs." ),
		_T( "(Drs." ),
		_T( "(Drs.)" ),
		_T( "Drs.)" ),

		_T( "Miss" ),
		_T( "(Miss" ),
		_T( "(Miss)" ),
		_T( "Miss)" ),
	};

	for ( auto& node : arrValues )
	{
		shared_ptr<int> pCount = shared_ptr<int>
		(
			new int( 1 )
		);
		m_titles.add( node, pCount );
	}
} // BuildTitleData

/////////////////////////////////////////////////////////////////////////////
// determine if the input is a known title like Mr., (Mr., Mr.), or (Mr.)
bool IsTitle( CString input )
{
	bool value = false;

	// trim some characters except period since period is part of Mr. and Mrs.
	input.Trim( _T( ",;:- " ) );
	value = m_titles.Exists[ input ];

	// if the value is false, trim the period for cases like Uncle.
	if ( !value )
	{
		input.Trim( _T( "." ) );
		value = m_titles.Exists[ input ];
	}

	return value;
} // IsTitle 

/////////////////////////////////////////////////////////////////////////////
// a reference to a title with only a last name is not valid
bool InvalidTitle( vector<CString>& Output )
{
	bool value = false;
	const size_t nTokens = Output.size();
	if ( nTokens == 2 )
	{
		Output[ 0 ].Trim();
		if ( IsTitle( Output[ 0 ] ) )
		{
			value = true;
		}
	}

	return value;
} // InvalidTitle

/////////////////////////////////////////////////////////////////////////////
// build a map of words to ignore given the path of the executable
// which is used to locate the Words.TXT file
bool BuildIgnoreWords( CString csPath, CKeyedCollection< CString, int >& words )
{
	bool bOK = false;
	const CString csFolder = CHelper::GetFolder( csPath );
	const CString csData = csFolder + _T( "words.txt" );
	if ( !::PathFileExists( csData ) )
	{
		return bOK;
	}

	// open the words file and error out on failure
	CStdioFile file;
	if ( !file.Open( csData, CFile::modeRead | CFile::typeText ) )
	{
		return bOK;
	}

	do
	{
		// read a line of text from the words.txt file
		CString csLine;
		if ( !file.ReadString( csLine ) )
		{
			break;
		}

		// trim whitespace from both ends
		csLine.Trim();


		// ignore empty lines
		if ( csLine.IsEmpty() )
		{
			continue;
		}

		// leading character of the word
		const TCHAR cFirst = csLine[ 0 ];
		const size_t nLen = csLine.GetLength();

		// test for lower case word
		bool bLower = cFirst >= 'a' && cFirst <= 'z';
		if ( !bLower )
		{
			if ( cFirst == '(' && nLen > 1 )
			{
				const TCHAR cSecond = csLine[ 1 ];
				bLower = cSecond >= 'a' && cSecond <= 'z';
			}
		}

		// ignore if not lower case
		if ( !bLower )
		{
			continue;
		}

		// if the word is unique, add it to the
		// collection, otherwise increment the 
		// count of items found
		shared_ptr<int> pCount;
		if ( words.Exists[ csLine ] )
		{
			pCount = words.find( csLine );
			*pCount += 1;
		}
		else // add the word to our collection
		{
			pCount = shared_ptr<int>
			(
				new int( 1 )
			);
			words.add( csLine, pCount );
		}
	}
	while ( true );

	bOK = words.Count > 0;
	return bOK;

} // BuildIgnoreWords

/////////////////////////////////////////////////////////////////////////////
// create a collection of name suffixes
void BuildSuffixData()
{
	CString arrValues[] = 
	{
		_T( "Jr." ),
		_T( "Sr." ),
		_T( "O.P." ),
		_T( "II" ),
		_T( "III" ),
		_T( "IV" ),
		_T( "V" ),
		_T( "VI" ),
		_T( "VII" ),
		_T( "VIII" ),
		_T( "MD" ),
		_T( "PhD" ),
	};

	for ( auto& node : arrValues )
	{
		shared_ptr<int> pCount = shared_ptr<int>
		(
			new int( 1 )
		);
		m_suffixes.add( node, pCount );
	}
} // BuildSuffixData

/////////////////////////////////////////////////////////////////////////////
// test for a name suffix
bool IsSuffix( CString input )
{
	bool value = false;

	// trim some characters except period since period is part of Jr. and Sr.
	input.Trim( _T( ",;:- " ));
	value = m_suffixes.Exists[ input ];

	// if the value is false, trim the period for cases like III.
	if ( !value )
	{
		input.Trim( _T( "." ) );
		value = m_suffixes.Exists[ input ];
	}

	return value;
} // IsSuffix

/////////////////////////////////////////////////////////////////////////////
// a period terminates a sequence
bool IsPeriod( CString& input )
{
	bool value = false;

	// titles ending in a period are not terminations
	if ( IsTitle( input ) )
	{
		return value;
	}

	const CString csPeriodQuote = _T( ".â€\x9d" );

	const int nLen = input.GetLength();
	int nMinLen = 2;
	if ( nLen > 0 )
	{
		// allow for an open parenthesis
		if ( input[ 0 ] == '(' )
		{
			nMinLen++;
		}

		// allow for a close parenthesis
		if ( input[ nLen - 1 ] == ')' )
		{
			nMinLen++;
		}

		// not an initial like H. or (H. 
		if ( nLen > nMinLen )
		{
			if ( input[ nLen - 1 ] == '.' )
			{
				input.TrimRight( _T( "." ) );
				value = true;
			} 
			else
			{
				// test for Unicode period followed by a close quote (.") 
				const CString csRight = input.Left( 4 );
				if ( csRight == csPeriodQuote )
				{
					input.TrimRight( csPeriodQuote );
					value = true;
				}
			}

		}
	}

	return value;
} // IsPeriod

/////////////////////////////////////////////////////////////////////////////
// strips plural suffix from the given word
bool HandlePlural( CString& word )
{
	bool value = true;

	// Unicode accent character
	if ( word.Right( 4 ) == m_csAccent + _T( "s" ) )
	{
		word.TrimRight( m_csAccent + _T( "s" ) );
	}
	else if ( word.Right( 4 ) == m_csAccent + _T( "S" ) )
	{
		word.TrimRight( m_csAccent + _T( "S" ) );
	}

	// ASCII accent character
	else if ( word.Right( 2 ) == _T( "'s" ) )
	{
		word.TrimRight( _T( "'s" ) );
	}
	else if ( word.Right( 2 ) == _T( "'S" ) )
	{
		word.TrimRight( _T( "'S" ) );
	}
	else
	{
		value = false;
	}

	return value;
} // HandlePlural

/////////////////////////////////////////////////////////////////////////////
// returns true if the word should be ignored, i.e. 
// not included in the output vector
bool IgnoreWord( CString word )
{
	bool value = false;

	// ignore empty lines or single characters
	int nLen = word.GetLength();
	if ( nLen < 2 )
	{
		value = true;
		return value;
	}

	// ignore lowercase words
	const bool bUpperCase = IsUppercase( word );
	if ( !bUpperCase )
	{
		value = true;
		return value;
	}

	// titles are automatically allowed
	const bool bTitle = IsTitle( word );
	if ( bTitle )
	{
		return value;
	}

	// suffixes are automatically allowed
	const bool bSuffix = IsSuffix( word );
	if ( bSuffix )
	{
		return value;
	}

	// initials are automatically allowed
	const bool bInitial = IsAnInitial( word );
	if ( bInitial )
	{
		return value;
	}

	CString csLower = word;
	csLower.MakeLower();

	// ignore if the lowercase version is in the  collection
	value = m_Ignore.Exists[ csLower ];
	if ( !value )
	{
		csLower.Trim
		(
			m_csAccent + m_csOpenQuote + m_csCloseQuote + _T( "\"(.',;)" )
		);

		// single letters are ignored
		nLen = csLower.GetLength();
		value = nLen < 2;

		// if still not being ignored, test to see if 
		// the word is in the ignore word collection
		if ( !value )
		{
			value = m_Ignore.Exists[ csLower ];
		}
	}

	return value;
} // IgnoreWord

/////////////////////////////////////////////////////////////////////////////
// add right column of concordance table where the input is the left
// side of the table and the output is both columns of the table
// separated by a tilde (~) character. The second column is: 
//	"last name,first name"
CString BuildConcordance( CString input )
{
	CString value = input;

	// break the string up into tokens
	vector<CString> tokens;
	int nStart = 0;
	do
	{
		const CString csToken = input.Tokenize( _T( " " ), nStart );
		if ( csToken.IsEmpty() )
		{
			break;
		}

		tokens.push_back( csToken );
	}
	while ( true );

	// determine what words are output first for the 
	// second column. Usually the last name, but since
	// there can be suffix, the last name and the suffix
	// are output first: 
	//		last_name suffix, first name [middle_name]...

	const size_t nLast = tokens.size() - 1;
	size_t nWord = nLast;
	bool bFound = false;

	// find the index of the last name
	do
	{
		CString csWord = tokens[ nWord ];
		csWord.Trim( _T( "()," ));
		if ( IsSuffix( csWord ) )
		{
			if ( nWord == 0 )
			{
				break;
			}
			nWord--;
			continue;
		}
		else if ( IsAnInitial( csWord ) )
		{
			if ( nWord == 0 )
			{
				break;
			}
			nWord--;
			continue;
		}

		bFound = true;
	}
	while ( !bFound );

	// write the contents of column 2 of the concordance table
	if ( bFound )
	{
		// value contains column 1, appending a column separator 
		// which is a tilde character
		value += _T( "~" );

		// generate column 2 by writing out the last name and
		// optional title 
		CString Col2;

		// write the last name first
		for ( size_t nIndex = nWord; nIndex <= nLast; nIndex++ )
		{
			Col2 += tokens[ nIndex ];
			Col2 += " ";
		}

		// separate the last name from the first with a comma
		Col2.Trim();
		Col2 += _T( ", " );

		// write the first given names
		for ( size_t nIndex = 0; nIndex < nWord; nIndex++ )
		{
			Col2 += tokens[ nIndex ];
			Col2 += " ";
		}

		Col2.Trim();
		
		// since column 2 is the text that appears in the index,
		// remove any parenthesis or quotes
		Col2.Remove( '(' );
		Col2.Remove( ')' );
		
		// remove open and close quotes
		for ( int nChar = 0; nChar < 3; nChar++ )
		{
			Col2.Remove( m_csOpenQuote[ nChar ] );
			Col2.Remove( m_csCloseQuote[ nChar ] );
		}

		// append column 2 to column 1
		value += Col2;
	}

	return value;
} // BuildConcordance

/////////////////////////////////////////////////////////////////////////////
// collect the output string into the total sorted output collection
void AddOutputData
( 
	CString csOutput
)
{
	// these are examples of comma separators we want to protect
	// by replacing the commas with a pipe character
	csOutput.Replace( _T( ", Sr." ), _T( "| Sr." ) );
	csOutput.Replace( _T( ", Jr." ), _T( "| Jr." ) );

	// handle the case of multiple tokens separated by commas
	// and semicolons by first collecting all of the full names
	vector<CString> fullNames;
	int nStart = 0;
	const CString csDelim( _T( ",;" ));
	do
	{
		CString csToken = csOutput.Tokenize( csDelim, nStart );
		if ( csToken.IsEmpty() )
		{
			break;
		}
		csToken.Trim( _T( "-: " ));
		if ( csToken.IsEmpty() )
		{
			continue;
		}

		// there should be at least two words separated by spaces
		int nPos = csToken.FindOneOf( _T( " " ));
		if ( nPos == -1 )
		{
			continue;
		}

		// replace any pipe characters with commas
		csToken.Replace( '|', ',' );

		// collect the full names
		fullNames.push_back( csToken );

	}
	while( true );

	// process the full names by adding unique ones to the 
	// total output collection
	const size_t nFullNames = fullNames.size();
	for ( auto& csFullName : fullNames )
	{
		// make sure the last name in this full name is not an initial
		// beginning with parsing the full name into individual words
		int nStart = 0;
		vector<CString> words;
		do
		{
			const CString csWord = csFullName.Tokenize( _T( " " ), nStart );
			if ( csWord.IsEmpty() )
			{
				break;
			}

			words.push_back( csWord );
		}
		while ( true );

		// the number of words in the in this full name
		const size_t nWords = words.size();

		// a list of names will very often only have a first
		// and middle name, so only accept a minimum of three
		// names if this is a list (more than one full name)
		if ( nFullNames > 1 && nWords < 3 )
		{
			continue;
		}

		// the last name of an initial is not allowed
		if ( IsAnInitial( words[ nWords - 1 ] ) )
		{
			continue;
		}

		// if the output is unique, add it to the
		// collection, otherwise increment the 
		// count of items found
		shared_ptr<int> pCount;
		if ( m_TotalOutput.Exists[ csFullName ] )
		{
			pCount = m_TotalOutput.find( csFullName );
			*pCount += 1;
		}
		else
		{
			pCount = shared_ptr<int>( new int( 1 ) );
			m_TotalOutput.add( csFullName, pCount );
		}
	}

} // AddOutputData

/////////////////////////////////////////////////////////////////////////////
// build the output string
void  BuildOutputString
( 
	CString& csOutput,
	vector<CString>& Output
)
{
	// an unclosed parenthesis is the sign of a problem, 
	// so remove all words beginning with the open parenthesis
	// word.
	HandleUnclosedParenthesis( Output );

	// append the capital words separated by spaces
	int nCount = 0;
	for ( auto& node : Output )
	{
		if ( IgnoreWord( node ) )
		{
			continue;
		}
		csOutput += node;
		csOutput += _T( " " );
		nCount++;
	}

	// trim trailing characters not associated with a name
	csOutput.TrimRight( _T( " 0123456789;,:\"" ) );
	const CString csRight = csOutput.Right( 3 );

	// special handling for trimming a trailing period
	// to account for the valid suffixes of Sr. and Jr.
	if 
	( 
		csRight != _T( "O.P." ) &&
		csRight != _T( "Sr." ) &&
		csRight != _T( "Jr." )
	)
	{
		csOutput.TrimRight( _T( "." ));
	}
	
	// single tokens cannot be a full name
	if ( nCount < 2 )
	{
		Output.clear();
		csOutput.Empty();
		return;
	}

	// collect the output
	AddOutputData( csOutput );

	// clear the output buffers to start again
	Output.clear();
	csOutput.Empty();
} // BuildOutputString

/////////////////////////////////////////////////////////////////////////////
// read the "names.txt" file
void ReadNames( CStdioFile& file )
{
	// read a single line of text
	CString csLine;
	do
	{
		// read a single line of text
		CString csLine;
		if ( !file.ReadString( csLine ) )
		{
			break;
		}

		// length of the line in characters
		size_t nLen = csLine.GetLength();
		if ( nLen == 0 )
		{
			continue;
		}

		// if the output is unique, add it to the
		// collection, otherwise increment the 
		// count of items found
		shared_ptr<int> pCount;
		if ( m_TotalOutput.Exists[ csLine ] )
		{
			pCount = m_TotalOutput.find( csLine );
			*pCount += 1;
		}
		else // add the new name
		{
			pCount = shared_ptr<int>
			(
				new int( 1 )
			);
			m_TotalOutput.add( csLine, pCount );
		}
	}
	while ( true );
} // ReadNames

/////////////////////////////////////////////////////////////////////////////
// read the words file named in the first parameter of the program
int ReadWords( CStdioFile& file )
{
	// crawl through the input file parsing the words
	// and build a corresponding line of output for each
	// word that begins with a capital letter
	CString csOutput;
	vector<CString> Output;

	// build a collection of words to ignore
	if ( !BuildIgnoreWords( m_arrArgs[ 0 ], m_Ignore ) )
	{
		return 6;
	}

	do
	{
		// read a single line of text
		CString csLine;
		if ( !file.ReadString( csLine ) )
		{
			break;
		}

		// length of the line in characters
		size_t nLen = csLine.GetLength();

		// trim characters
		const CString csTrim( _T( "-'\": " ) );

		// comma separated token
		if ( nLen == 2 && csLine[ 1 ] == ',' )
		{
			continue;
		}
		else
		{
			csLine.Trim( csTrim );
		}

		// test for non-capitalized word and if true
		// the text will be excluded from the output
		bool bExclude = IgnoreWord( csLine );

		// if the word is being excluded from the output,
		// we need to output what has already been collected
		if ( bExclude )
		{
			const size_t nTokens = Output.size();
			if ( nTokens == 0 )
			{
				csOutput.Empty();
				continue;
			}

			// single tokens cannot be a full name
			if ( nTokens == 1 )
			{
				Output.clear();
				csOutput.Empty();
				continue;
			}

			// a reference to a title with only a last name
			// is not used
			const bool bInvalidTitle = InvalidTitle( Output );
			if ( bInvalidTitle )
			{
				Output.clear();
				csOutput.Empty();
				continue;
			}

			// strip enclosed parenthesis and or quotes if they exist
			StripEnclosures( Output );

			// all initials is invalid
			const bool bInvalidInitials = InvalidInitials( Output );
			if ( bInvalidInitials )
			{
				Output.clear();
				csOutput.Empty();
				continue;
			}

			// strip trailing period
			StripPeriod( Output );

			BuildOutputString( csOutput, Output );

		}
		else // include text
		{
			// look for terminators that end the name
			const bool bSuffix = IsSuffix( csLine );

			// is the word an initial?
			const bool bInitial = IsAnInitial( csLine );

			// a period could be a terminator
			bool bPeriod = false;
			if ( !bSuffix && !bInitial )
			{
				bPeriod = IsPeriod( csLine );
			}

			// if the line is not terminated with a period
			// then handle the case of a plural word (*'s, *'S)
			// by stripping off the plural suffix
			if ( !bPeriod )
			{
				HandlePlural( csLine );
			}

			// add the line to the output array
			Output.push_back( csLine );

			// if the current data is a suffix, terminate
			// the collection
			if ( bSuffix )
			{
				const size_t nWords = Output.size();

				// if there are only one of two (name, Sr.) words, then there
				// is not a first and last name
				if ( nWords > 2 )
				{
					// strip enclosed parenthesis and or quotes if they exist
					StripEnclosures( Output );

					// the current word is terminating the output if
					// it ends in a period
					BuildOutputString( csOutput, Output );

					// collect the output
					AddOutputData( csOutput );
				}

				// clear the output buffers to start again
				Output.clear();
				csOutput.Empty();
			}
			else if ( bPeriod )
			{
				// strip enclosed parenthesis and or quotes if they exist
				StripEnclosures( Output );

				// the current word is terminating the output if
				// it ends in a period
				BuildOutputString( csOutput, Output );
			}
			else // handle a special case where a previous name gets appended to
			{
				const size_t nWords = Output.size();
				if ( nWords == 3 )
				{
					CString csKey;
					for ( auto& node : Output )
					{
						csKey += node;
						csKey += _T( " " );
					}

					csKey.Trim();

					// if the name already exists, clear the buffers
					if ( m_TotalOutput.Exists[ csKey ] )
					{
						// clear the output buffers to start again
						Output.clear();
						csOutput.Empty();
					}
				}
			}
		}
	}
	while ( true );

	return 0;
} // ReadWords

/////////////////////////////////////////////////////////////////////////////
// a console application that can crawl through the file
// and build a sorted list of filenames
int _tmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
	// by default, a concordance table is not output
	m_bConcordance = false;

	// if the input file is called "names.txt", then it is assumed to be the 
	// output of this program when m_bConcordance is false, 
	// i.e. First [Middle] Last is the input instead of individual words
	// that is normally the input
	m_bNames = false;

	HMODULE hModule = ::GetModuleHandle( NULL );
	if ( hModule == NULL )
	{
		_tprintf( _T( "Fatal Error: GetModuleHandle failed\n" ) );
		return 1;
	}

	// initialize MFC and error on failure
	if ( !AfxWinInit( hModule, NULL, ::GetCommandLine(), 0 ) )
	{
		_tprintf( _T( "Fatal Error: MFC initialization failed\n " ) );
		return 2;
	}

	// do some common command line argument corrections
	m_arrArgs = CHelper::CorrectedCommandLine( argc, argv );
	const size_t nArgs = m_arrArgs.size();

	// this stream can be redirected from the command line to allow the 
	// output you are interested in to be captured into another file
	// (Ex. > out_file.csv)
	CStdioFile fOut( stdout );

	// this stream is not redirected; it only shows up on the console and
	// will not affect the output file that is being redirected to
	CStdioFile fErr( stderr );

	CString csMessage;

	// display the number of arguments if not 1 or 2 to help the user 
	// understand what went wrong if there is an error in the
	// command line syntax
	bool bUsage = false;
	if ( nArgs == 2 || nArgs == 3 )
	{
		fErr.WriteString( _T( ".\n" ) );
		csMessage.Format( _T( "The number of parameters are %d\n.\n" ), nArgs - 1 );
		fErr.WriteString( csMessage );

		// display the arguments
		for ( size_t i = 1; i < nArgs; i++ )
		{
			csMessage.Format( _T( "Parameter %d is %s\n.\n" ), i, m_arrArgs[ i ] );
			fErr.WriteString( csMessage );
		}
	}
	else // give the user some usage information
	{
		bUsage = true;
	}

	// two arguments expected
	if ( bUsage )
	{
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString
		(
			_T( "CapitalWords, Copyright (c) 2022, " )
			_T( "by W. T. Block.\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "A Windows command line program to read a text\n" )
			_T( "  file of lines containing single words and \n" )
			_T( "  output a text file of the words\n" )
			_T( "  beginning with a capital letter.\n" )
			_T( ".\n" )
			_T( "The input text file is in this format:\n" )
			_T( "  single_word\n" )
			_T( ".\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "Usage:\n" )
			_T( ".\n" )
			_T( ".  CapitalWords input_file [concordance]\n" )
			_T( ".\n" )
			_T( "Where:\n" )
			_T( ".\n" )
			_T( ".  input_file is the pathname of the input file.\n" )
			_T( ".  concordance is optional true/false (default).\n" )
			_T( ".		to determine the type of output:\n" )
			_T( ".			false - (default) yields a single column of names\n" )
			_T( ".			true  - yields two columns separated by tilde (~)\n" )
			_T( ".					which can be pasted into a Word document\n" )
			_T( ".					to create a concordance table for importing\n" )
			_T( ".					index markings into another Word document:\n" )
			_T( ".						\"original_name~last_name, first_name\n" )
			_T( ".					where original_name is the text from the input\n" )
			_T( ".					and last_name, first_name is the text in the index.\n" )
			_T( ".\n" )
			_T( "NOTE: a special case is where the input_file is \"names.txt\" which\n" )
			_T( ".	is in the format of the standard output when concordance is\n" )
			_T( ".	set to false. In this case, the program will output a concordance\n" )
			_T( ".	table using the names.txt file as its input. \n" )
			_T( ".\n" )
		);

		return 3;
	}

	// display the executable path
	csMessage.Format( _T( "Executable pathname: %s\n" ), m_arrArgs[ 0 ] );
	fErr.WriteString( _T( ".\n" ) );
	fErr.WriteString( csMessage );
	fErr.WriteString( _T( ".\n" ) );

	// retrieve the pathname
	const CString csInput = m_arrArgs[ 1 ];

	// test to see if there is an optional parameter to control the type
	// of output and if "true" mark the output type to be concordance
	CString csConcordance = _T( "false" );
	if ( nArgs == 3 )
	{
		csConcordance = m_arrArgs[ 2 ];
		csConcordance.MakeLower();
		if ( csConcordance == _T( "true" ) )
		{
			m_bConcordance = true;
		}
	}

	// check for file existence
	bool bExists = false;
	if ( ::PathFileExists( csInput ) )
	{
		bExists = true;
	}

	// error out if the file does not exist
	if ( !bExists )
	{
		csMessage.Format( _T( "Invalid input file:\n\t%s\n" ), csInput );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 4;

	}
	else // display the name of the file input
	{
		csMessage.Format( _T( "Given input:\n\t%s\n" ), csInput );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
	}

	// open the file and error out on failure
	CStdioFile file;
	if ( !file.Open( csInput, CFile::modeRead | CFile::typeText ) )
	{
		csMessage.Format( _T( "Invalid input file:\n\t%s\n" ), csInput );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 5;
	}

	// special case of the input file being named "names.txt" which will
	// be used to directly create a concordance file
	const CString csFilename = CHelper::GetDataName( csInput ).MakeLower();
	if ( csFilename == _T( "names.txt" ) )
	{
		m_bNames = true;
		m_bConcordance = true;
	}

	// collection of suffixes like Jr., Sr., III, etc.
	BuildSuffixData();

	// create a collection of name titles (prefixes)
	BuildTitleData();

	// building concordance file directly?
	if ( m_bNames == true )
	{
		// read names reads the "names.txt" file that contains
		// the output of this program when the concordance
		// parameter is set to false. Every line of this file
		// contains names from the target Microsoft Word file
		// in the format of: 
		//		first_name [middle name]... last_name
		// where there is expected to be a minimum of two words
		// for the first and last names
		ReadNames( file );
	} 
	else 
	// reading the normal input file containing words from 
	// the target Microsoft Word file to be indexed
	{
		int nReturn = ReadWords( file );
		if ( nReturn != 0 )
		{
			csMessage.Format
			( 
				_T( "Failed to read the \"words.txt\" file\n" )
				_T( ".	which is located in the folder with the\n" )
				_T( ".	with executable. This file contains words\n" )
				_T( ".	from the English language that are not names\n" )
				_T( ".	if they are lowercase.\n" )
			);
			fErr.WriteString( _T( ".\n" ) );
			fErr.WriteString( csMessage );
			fErr.WriteString( _T( ".\n" ) );
			return nReturn;
		}
	}
	
	// output to the console the unique line we found
	for ( auto& node : m_TotalOutput.Items )
	{
		const CString csToken = node.first;

		// default to outputting the token that was originally stored
		CString csOut = csToken;

		// add right column of concordance table if the user
		// added the second parameter of "true"
		if ( m_bConcordance == true )
		{
			csOut = BuildConcordance( csToken );
		}

		// write the output line to the console (which
		// can be redirected to another text file)
		fOut.WriteString( csOut + _T( "\n" ));
	}

	// all is good
	return 0;

} // _tmain
