// SketchOrder.h

#pragma once

enum SketchOrderType
{
	SketchOrderTypeEmpty,	// an empty sketch
	SketchOrderTypeOpen,	// a well ordered sketch, which doesn't have it's start and end touching
	SketchOrderTypeReverse,	// reverse the sketch, only used as a function, won't be returned by GetSketchOrder()
	SketchOrderTypeBad,	// "badly ordered" ( where the start of one span doesn't line up with the end of the previous span )
	SketchOrderTypeReOrder,	// re-order the sketch, only used as a function, won't be returned by GetSketchOrder()
	SketchOrderTypeCloseCW,	// a well ordered sketch, which is closed and clockwise
	SketchOrderTypeCloseCCW,// a well ordered sketch, which is closed and anti-clockwise
	MaxSketchOrderTypes
};
