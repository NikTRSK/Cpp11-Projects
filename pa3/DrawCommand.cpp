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
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset(); // do i need this
}

void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
//	if (model->CanUndo())
//	{
		model->GetShapes().pop_back();
		model->Undo();
//	}
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
//	if (model->CanRedo())
//	{
	model->GetShapes().push_back(model->GetTopInRedo()->GetShape());
		model->Redo();
//	}
}
