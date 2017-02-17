#include "DrawCommand.h"

DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{

}


DrawCommand::~DrawCommand()
{
}

void DrawCommand::Update(const wxPoint& newPoint)
{
	Command::Update(newPoint);
	mShape->Update(newPoint);
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mShape->Finalize();

//	Command::Finalize(model);
}

void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
}
