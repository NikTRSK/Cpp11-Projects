#include "MoveCommand.h"
#include <iostream>


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
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	wxPoint ts = model->GetTopInUndo()->GetShape()->GetOffset();
	model->Undo();
//	model->GetCurrentCommand();
	wxPoint t = model->GetTopInUndo()->GetShape()->GetOffset();
	
	std::cout << "draw" << t.x << ", " << t.y << " move: " << ts.x << ", " << ts.y << std::endl;
//	mShape->UpdateOffset(-t);
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
}
