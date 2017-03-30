#include "World.h"
#include "Exceptions.h"
//#include <wx/msgdlg.h>
//#include <wx/defs.h>



World::World()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}


World::~World()
{
}

void World::AddZombie(const Machine<ZombieTraits> & zombie)
{
	mZombieStates.push_back(MachineState());
	mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombie));
	mZombies.back()->BindState(mZombieStates.back());
}

void World::GenerateZombies(const Machine<ZombieTraits> & zombie)
{
//	MachineState state;
//	state.mFacing = static_cast<MachineState::Facing>(rand() % 4);
//	int x = 5, y = 3;
//	state.mCoordinate->x = x;
//	state.mCoordinate->y = y;
//	mZombieStates.push_back(state);
//	mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombie));
//	mZombies.back()->BindState(mZombieStates.back());
//	mGridZombies[x][y] = &mZombieStates.back();





	for (auto i = 0; i < 20; ++i)
	{
		MachineState state;
		state.mFacing = static_cast<MachineState::Facing>(rand() % 4);
		int x, y;
		do
		{
			x = rand() % 20;
			y = rand() % 20;
		} while (mGridZombies[x][y] != nullptr || mGridHumans[x][y] != nullptr);
		state.mCoordinate->x = x;
		state.mCoordinate->y = y;
//		std::cout << "X: " << x << " Y: " << y << std::endl;
		mZombieStates.push_back(state);
		mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombie));
		mZombies.back()->BindState(mZombieStates.back());
//		mGridZombies[x][y] = mZombies.back();
		mGridZombies[x][y] = &mZombieStates.back();
	}
}

void World::GenerateHumans(const Machine<HumanTraits>& human)
{

//	MachineState state2;
//	state2.mFacing = static_cast<MachineState::Facing>(rand() % 4);
//	int x = 5, y = 5;
//	state2.mCoordinate->x = x;
//	state2.mCoordinate->y = y;
//	mHumanStates.push_back(state2);
//	mHumans.push_back(std::make_shared<Machine<HumanTraits>>(human));
//	mHumans.back()->BindState(mHumanStates.back());
//	mGridHumans[x][y] = &mHumanStates.back();



	for (auto i = 0; i < 10; ++i)
	{
		MachineState state;
		state.mFacing = static_cast<MachineState::Facing>(rand() % 4);
		int x, y;
		do
		{
			x = rand() % 20;
			y = rand() % 20;
		} while (mGridHumans[x][y] != nullptr || mGridZombies[x][y] != nullptr);
		state.mCoordinate->x = x;
		state.mCoordinate->y = y;
//		std::cout << "X: " << x << " Y: " << y << std::endl;
		mHumanStates.push_back(state);
		mHumans.push_back(std::make_shared<Machine<HumanTraits>>(human));
		mHumans.back()->BindState(mHumanStates.back());
//		mGridHumans[x][y] = mHumans.back();
		mGridHumans[x][y] = &mHumanStates.back();
	}
}


std::vector<std::shared_ptr<Machine<ZombieTraits>>> World::GetZombies() const
{
	return mZombies;
}

std::vector<std::shared_ptr<Machine<HumanTraits>>> World::GetHumans() const
{
	return mHumans;
}

std::vector<MachineState> World::GetZombieStates() const
{
	return mZombieStates;
}

std::vector<MachineState> World::GetHumanStates() const
{
	return mHumanStates;
}

void World::UpdateWorld()
{
	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		try
		{
			mZombies.at(i)->TakeTurn(mZombieStates.at(i));
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox("Error", "Error", wxOK | wxICON_ERROR);
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
	}

	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		try
		{
			mHumans.at(i)->TakeTurn(mHumanStates.at(i));
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
//			wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		}
	}
}

void World::ClearData()
{
	mZombies.clear();
	mZombieStates.clear();
	mHumanStates.clear();
	mHumans.clear();
	for (auto i = 0; i < mWorldSize; ++i)
	{
		for (auto j = 0; j < mWorldSize; ++j)
		{
			mGridZombies[i][j] = nullptr;
			mGridHumans[i][j] = nullptr;
		}
	}
}

bool World::HasHuman(const int& x, const int& y) const noexcept
{
	return mGridHumans[x][y] != nullptr;
}

bool World::HasZombie(const int& x, const int& y) const noexcept
{
	return mGridZombies[x][y] != nullptr;
}

void World::KillZombie(MachineState& state, int offset) noexcept
{
	MachineState *zombieToKill = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		zombieToKill = mGridZombies[x][y - offset];
		mGridZombies[x][y - offset] = nullptr;
		break;
	case MachineState::DOWN:
		zombieToKill = mGridZombies[x][y + offset];
		mGridZombies[x][y + offset] = nullptr;
		break;
	case MachineState::LEFT:
		zombieToKill = mGridZombies[x - offset][y];
		mGridZombies[x - offset][y] = nullptr;
		break;
	case MachineState::RIGHT:
		zombieToKill = mGridZombies[x + offset][y];
		mGridZombies[x + offset][y] = nullptr;
		break;
	default:
		break;
	}

//	if (mGridZombies[x][y] == nullptr)
//		return;
	for (unsigned int i = 0; i < mZombieStates.size(); ++i)
	{
		if (&mZombieStates[i] == zombieToKill)
		{
			std::cout << "Killing zombie\n";
			mZombies.erase(mZombies.begin() + i);
			mZombieStates.erase(mZombieStates.begin() + i);
			return;
		}
	}
}

void World::KillHuman(MachineState& state, int offset) noexcept
{
	MachineState *humanToKill = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToKill = mGridHumans[x][y - offset];
		mGridHumans[x][y - offset] = nullptr;
		break;
	case MachineState::DOWN:
		humanToKill = mGridHumans[x][y + offset];
		mGridHumans[x][y + offset] = nullptr;
		break;
	case MachineState::LEFT:
		humanToKill = mGridHumans[x - offset][y];
		mGridHumans[x - offset][y] = nullptr;
		break;
	case MachineState::RIGHT:
		humanToKill = mGridHumans[x + offset][y];
		mGridHumans[x + offset][y] = nullptr;
		break;
	default:
		break;
	}

	//	if (mGridZombies[x][y] == nullptr)
	//		return;
	for (unsigned int i = 0; i < mHumanStates.size(); ++i)
	{
		if (&mHumanStates[i] == humanToKill)
		{
			std::cout << "Killing human\n";
			mHumans.erase(mHumans.begin() + i);
			mHumanStates.erase(mHumanStates.begin() + i);
			return;
		}
	}
}

void World::ConvertHuman(MachineState& state) noexcept
{
	MachineState *humanToConvert = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToConvert = mGridHumans[x][y - 1];
		mGridHumans[x][y - 1] = nullptr;
		break;
	case MachineState::DOWN:
		humanToConvert = mGridHumans[x][y + 1];
		mGridHumans[x][y + 1] = nullptr;
		break;
	case MachineState::LEFT:
		humanToConvert = mGridHumans[x - 1][y];
		mGridHumans[x - 1][y] = nullptr;
		break;
	case MachineState::RIGHT:
		humanToConvert = mGridHumans[x + 1][y];
		mGridHumans[x + 1][y] = nullptr;
		break;
	default:
		break;
	}

	for (unsigned int i = 0; i < mHumanStates.size(); ++i)
	{
		if (&mHumanStates[i] == humanToConvert)
		{
			std::cout << "Converting human\n";
			
			Machine<ZombieTraits> zombieMachine;
			zombieMachine.SetOps(mZombies.back()->GetOps());
			zombieMachine.BindState(*humanToConvert);
			mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombieMachine));
			mZombieStates.push_back(*humanToConvert);
			mHumans.erase(mHumans.begin() + i);
			mHumanStates.erase(mHumanStates.begin() + i);
			
			mGridZombies[x][y] = &mZombieStates.back();
			return;
		}
	}
}

void World::PrintWorld() const noexcept
{
	std::cout << "Zombie Grid\n";
	for (unsigned int i = 0; i < 20; ++i)
	{
		for (unsigned int j = 0; j < 20; ++j)
		{
			if (mGridZombies[i][j] == nullptr) 
			{
				std::cout << "0" << " | ";
			}
			else 
			{
				std::cout << "Z" << " | ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "Human Grid\n";
	for (unsigned int i = 0; i < 20; ++i)
	{
		for (unsigned int j = 0; j < 20; ++j)
		{
			if (mGridHumans[i][j] == nullptr)
			{
				std::cout << "0" << " | ";
			}
			else
			{
				std::cout << "H" << " | ";
			}
		}
		std::cout << std::endl;
	}
}
