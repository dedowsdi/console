#ifndef PACCONSOLEPATTERN_H
#define PACCONSOLEPATTERN_H 

#include "pacConsolePreRequisite.h"

namespace pac
{

/**
 * Interface of console output pattern. 
 */
class _PacExport ConsolePattern 
{
public:

	/**
	 * Ctor
	 * @param textWidth : line width
	 */
	ConsolePattern(size_t textWidth);
	virtual ~ConsolePattern(){}

	/**
	 * Apply pattern, get result string which will be output direct by console.
	 * @param beg : begin iter
	 * @param end : end iter 
	 * @return : formated string
	 */
	virtual std::string applyPattern(SVIter beg, SVIter end) = 0;
protected:

	size_t mTextWidth; //line width

};


/**
 * Default pattern. This pattern will sort input buffers, place buffer column by
 * column. last column might not full filled. Just like gnome. 
 */
class _PacExport DefaultPattern : public ConsolePattern
{
public:

	/**
	 * ctor
	 * @param textWidth : line width
	 * @param spacing : spacing between max item in current column and next column 
	 * @return : 
	 */
	DefaultPattern(size_t textWidth, size_t spacing = 2);

	virtual std::string applyPattern(SVIter beg,
			SVIter end); 

protected:

	/**
	 * Get length of shortest and longest item 
	 * @param beg : beg iterator
	 * @param end : end iterator 
	 * @param{out} minItemLen : result min length
	 * @param{out} maxItemLen : result max length 
	 */
	void getMinMaxItemLen(SVIter beg, SVIter end, size_t& minItemLen, size_t& maxItemLen);

	/**
	 * Get candidate column numbers. Every column except last one must be
	 * fullfilled , which means the following equation must be satisfied:
	 *
	 * 		numItem % numCol < ceil(float(numItem) / numCol)
	 *
	 * @param numItem : count of itms
	 * @param minItemLen : min item length 
	 * @return : size_t vector of candidate column numbers in asec order.
	 */
	SizetVector getCandidateNumColumns(size_t numItem, size_t minItemLen, size_t maxItemLen); 

	/**
	 * Check if column number is valid. It's valid if total column width is
	 * smaller than line width;
	 * @param beg : begin iter
	 * @param end : end iter
	 * @param numCol : number of columns
	 * @param{out} colWidthes : column width of each column
	 * @return : true if it's valid, otherwise false;
	 */
	bool valid(SVIter beg, SVIter end, size_t numCol, SizetVector& colWidthes);

protected:
	
	size_t mSpacing;

};


}


#endif /* PACCONSOLEPATTERN_H */
