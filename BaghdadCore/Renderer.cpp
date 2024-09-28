#include "Renderer.h"

#include "BaghdadError.h"

using namespace BaghdadCore;

Renderer::Renderer()
{
	auto cards = _factory.EnumGraphicCards();
	if (cards.size() < 1)
	{
		THROW_BERROR("No Graphics Capable Adapter (Graphics Card) found !");
	}

	_pCard = std::make_unique<GraphicsCard>(std::move(cards[0]));
	_pDevice = std::make_unique<Device>(*_pCard);
	_pMaterialBuilder = std::make_unique<MaterialBuilder>(*_pDevice);
}