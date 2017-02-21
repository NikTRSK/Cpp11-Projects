#include "MoveCommand.h"


MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


MoveCommand::~MoveCommand()
{
}

void MoveCommand::Update(const wxPoint & newPoint)
{
	mShape->UpdateOffset(newPoint);
}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	model->GetCurrentCommand().reset();
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
}
