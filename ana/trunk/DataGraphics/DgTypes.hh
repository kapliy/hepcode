#ifndef WRAP_DGDATAGRAPHICSTYPES_HH
#define WRAP_DGDATAGRAPHICSTYPES_HH

namespace
DataGraphics
{
  // counter type is double which maintains sufficient precision for 2E10^16 - 1E10^16 = 1E0
  // instead of unsigned long which maintains unit precision throughout the smaller range 0 to ~4.29E9
  
  // bin size is unsigned short so that 2D histograms can have up to 65536x65536 = 4.29 billion bins
  // this requires that the histogram functions use a 2D bin index that can handle 65536^2, i.e. unsigned int
  typedef unsigned short  BinSize;
  typedef unsigned int    BinIndex;
  typedef double          Value;
  typedef double          Count;
  typedef double          WeightedCount; // changing this to double does not compile yet.

}; // end DataGraphics namespace

#endif // WRAP_DGDATAGRAPHICSTYPES_HH
