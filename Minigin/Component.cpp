#include "Component.h"
#include "Callback.h"


void dae::Component::SetCallback(Callback* const subject)
{
	m_pCallback = subject;
}