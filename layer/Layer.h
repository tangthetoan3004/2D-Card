#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <QColor>

struct Layer
{
	std::string name;
	QColor color;
	bool isVisible;
	bool isLocked;

	Layer(const std::string& n = "0", const QColor& c = Qt::black, bool vis = true, bool lock = false)
		: name(n), color(c), isVisible(vis), isLocked(lock)
	{
	}
};

class LayerManager
{
public:
	LayerManager();
	~LayerManager() = default;

	bool AddLayer(const std::string& name, const QColor& color = Qt::black);
	bool RemoveLayer(const std::string& name);
	std::shared_ptr<Layer> GetLayer(const std::string& name) const;
	std::vector<std::shared_ptr<Layer>> GetAllLayers() const;

	void SetActiveLayer(const std::string& name);
	std::shared_ptr<Layer> GetActiveLayer() const;
	std::string GetActiveLayerName() const;

	void SetLayerVisible(const std::string& name, bool visible);
	void SetLayerLocked(const std::string& name, bool locked);
	void SetLayerColor(const std::string& name, const QColor& color);

private:
	std::map<std::string, std::shared_ptr<Layer>> mLayers;
	std::string mActiveLayerName;
};
