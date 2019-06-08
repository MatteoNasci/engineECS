#include "Dispatcher.h"
engineECS::Dispatcher::Dispatcher()
{
	//spawn thread
}
engineECS::Dispatcher::~Dispatcher()
{

}
void engineECS::Dispatcher::submitTask(physx::PxBaseTask& task)
{
	task.run();
	task.release();
}

uint32_t engineECS::Dispatcher::getWorkerCount() const
{
	return 0;
}
