#include "Command.h"
#include "Shape.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineShape.h"
#include "PencilShape.h"
#include "PaintModel.h"
#include "DrawCommand.h"
#include "SetPenCommand.h"
#include "SetBrushCommand.h"

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Shape> Command::GetShape()
{
	return mShape;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
	std::shared_ptr<Shape> shape; // TODO: Should we use mShape instead
	
	// TODO: Actually create a command based on the type passed in
	switch (type)
	{
		case CM_DrawLine:
			shape = std::make_shared<LineShape>(start);
			retVal = std::make_shared<DrawCommand>(start, shape);
			model->AddShape(shape);
			break;
		
		case CM_DrawEllipse:
			shape = std::make_shared<EllipseShape>(start);
			retVal = std::make_shared<DrawCommand>(start, shape);
			model->AddShape(shape);
			break;

		case CM_DrawRect:
			shape = std::make_shared<RectShape>(start);
			retVal = std::make_shared<DrawCommand>(start, shape);
			model->AddShape(shape);
			break;

		case CM_DrawPencil:
			shape = std::make_shared<PencilShape>(start);
			retVal = std::make_shared<DrawCommand>(start, shape);
			model->AddShape(shape);
			break;

		case CM_Move:
		break;

		case CM_Delete: break;

		case CM_SetPen:
			retVal = std::make_shared<SetPenCommand>(start, shape);
			break;

		case CM_SetBrush:
			retVal = std::make_shared<SetBrushCommand>(start, shape); 
			break;

		default: break;
		
	}

	return retVal;
}