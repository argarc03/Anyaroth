#include "ShopEvent.h"

ShopEvent::ShopEvent(ShopMenu* shop, int currentZone) : _shop(shop)
{

}

void ShopEvent::update(const double& time)
{
	if (_shop->isClosed())
	{
		_finished = true;
	}
}

void ShopEvent::play()
{
	_shop->openShop();
}