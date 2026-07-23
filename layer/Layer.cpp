#include "Layer.h"

LayerManager::LayerManager()
{
	// Default layer 0 always exists
	AddLayer("0", Qt::black);
	mActiveLayerName = "0";
}

bool LayerManager::AddLayer(const std::string& name, const QColor& color)
{
	if (name.empty() || mLayers.find(name) != mLayers.end())
	{
		return false;
	}

	mLayers[name] = std::make_shared<Layer>(name, color, true, false);
	return true;
}

bool LayerManager::RemoveLayer(const std::string& name)
{
	// Layer 0 cannot be deleted
	if (name == "0" || mLayers.find(name) == mLayers.end())
	{
		return false;
	}

	mLayers.erase(name);
	if (mActiveLayerName == name)
	{
		mActiveLayerName = "0";
	}
	return true;
}

std::shared_ptr<Layer> LayerManager::GetLayer(const std::string& name) const
{
	auto it = mLayers.find(name);
	if (it != mLayers.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<std::shared_ptr<Layer>> LayerManager::GetAllLayers() const
{
	std::vector<std::shared_ptr<Layer>> result;
	for (const auto& pair : mLayers)
	{
		result.push_back(pair.second);
	}
	return result;
}

void LayerManager::SetActiveLayer(const std::string& name)
{
	if (mLayers.find(name) != mLayers.end())
	{
		mActiveLayerName = name;
	}
}

std::shared_ptr<Layer> LayerManager::GetActiveLayer() const
{
	return GetLayer(mActiveLayerName);
}

std::string LayerManager::GetActiveLayerName() const
{
	return mActiveLayerName;
}

void LayerManager::SetLayerVisible(const std::string& name, bool visible)
{
	auto layer = GetLayer(name);
	if (layer)
	{
		layer->isVisible = visible;
	}
}

void LayerManager::SetLayerLocked(const std::string& name, bool locked)
{
	auto layer = GetLayer(name);
	if (layer)
	{
		layer->isLocked = locked;
	}
}

void LayerManager::SetLayerColor(const std::string& name, const QColor& color)
{
	auto layer = GetLayer(name);
	if (layer)
	{
		layer->color = color;
	}
}
