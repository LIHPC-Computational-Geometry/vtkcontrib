#include "VtkContrib/vtkPolygonFilter.h"

#include <vtkDataSet.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#if VTK_MAJOR_VERSION >= 9
#include <vtkCellArrayIterator.h>
#endif	// VTK_MAJOR_VERSION >= 9
#include <assert.h>

#include <algorithm>
#include <list>
using namespace std;


vtkPolygonFilter::vtkPolygonFilter ( )
	: vtkPolyDataAlgorithm ( )
{
}	// vtkPolygonFilter::vtkPolygonFilter


vtkPolygonFilter::vtkPolygonFilter (const vtkPolygonFilter&)
	: vtkPolyDataAlgorithm ( )
{
	assert (0 && "vtkPolygonFilter copy constructor is not allowed.");
}	// vtkPolygonFilter::vtkPolygonFilter


vtkPolygonFilter& vtkPolygonFilter::operator = (const vtkPolygonFilter&)
{
	assert (0 && "vtkPolygonFilter operator = is not allowed.");
	return *this;
}	// vtkPolygonFilter::operator =


vtkPolygonFilter::~vtkPolygonFilter ( )
{
}	// vtkPolygonFilter::~vtkPolygonFilter


vtkPolygonFilter* vtkPolygonFilter::New ( )
{
	return new vtkPolygonFilter ( );
}	// vtkPolygonFilter::New


void vtkPolygonFilter::PrintSelf (ostream& os, vtkIndent indent)
{
	Superclass::PrintSelf(os,indent);
}       // vtkPolygonFilter::PrintSelf


#define RD_SUCCESS  1
#define RD_FAILURE  0
int vtkPolygonFilter::RequestData (vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
	vtkPolyData*	input		= GetPolyDataInput (0);
	vtkPolyData*	output		= GetOutput ( );
	vtkPoints*		pointSet	= 0 == input ? 0 : input->GetPoints ( );
	vtkCellArray*   cells		= 0 == input ? 0 : input->GetPolys ( );
	if (0 == output)
	{
		vtkErrorMacro(<<"vtkPolygonFilter::Execute : filter does not have any output.");
		return RD_FAILURE;
	}       // if (0 == output)
	if ((0 == input) || (0 == pointSet) || (0 == cells))
	{
		vtkErrorMacro(<<"vtkPolygonFilter::Execute : filter does not have any input.");
		return RD_FAILURE;
	}       // if (0 == input) || (0 == pointSet) || (0 == cells))
	output->Reset ( );

	const size_t	cellNum	= cells->GetNumberOfCells ( );
	output->SetPoints (pointSet);
	output->Allocate (cellNum, cellNum);
#ifdef VTK_CELL_ARRAY_V2	// defined in vtkCellArray.h	CP v 5.13.1 Portage VTK 9.3.0
	vtkSmartPointer<vtkCellArrayIterator> cellIterator = vtk::TakeSmartPointer (cells->NewIterator ( ));
	for (cellIterator->GoToFirstCell ( ); !cellIterator->IsDoneWithTraversal ( ); cellIterator->GoToNextCell ( ))
	{
		vtkIdType			nodeCount	= 0;
		const vtkIdType*	ids			= 0;
		cellIterator->GetCurrentCell (nodeCount, ids);
		switch (nodeCount)
		{
			case 3		: output->InsertNextCell (VTK_TRIANGLE, nodeCount, ids);		break;
			case 4		: output->InsertNextCell (VTK_QUAD, nodeCount, ids);			break;
		}	// switch (nodeCount)
	}	// for (cellIterator->GoToFirstCell ( ); !cellIterator->IsDoneWithTraversal ( ); cellIterator->GoToNextCell ( ))
#else	// VTK_CELL_ARRAY_V2
	vtkIdType*		cellsPtr	= cells->GetPointer ( );
	for (int i = 0; i < cellNum; i++)
	{
		const vtkIdType nodeCount	= *cellsPtr;
		cellsPtr++;
		switch (nodeCount)
		{
			case 3		: output->InsertNextCell (VTK_TRIANGLE, nodeCount, cellsPtr);		break;
			case 4		: output->InsertNextCell (VTK_QUAD, nodeCount, cellsPtr);			break;
		}	// switch (nodeCount)
		cellsPtr	+= nodeCount;
	}       // for (i = 0; i < cellNum; i++)
#endif	// VTK_CELL_ARRAY_V2

	output->GetCellData ( )->PassData (input->GetCellData ( ));
	output->Squeeze ( );

	return RD_SUCCESS;
}	// vtkPolygonFilter::RequestData
