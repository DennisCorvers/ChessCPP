#pragma once

class GuiBase;
class GuiManager : private sf::NonCopyable
{
private:
	friend class GuiBase;
	using GuiPair = std::pair<int, GuiBase*>;

	int m_nextGuiID;
	std::vector<GuiPair> m_guis;
	std::queue<int> m_toRemove;
	std::queue<int> m_toShow;
	std::queue<GuiBase*> m_toAdd;

public:
	GuiManager();
	virtual ~GuiManager();

	void update(float deltaTime) const;
	void render() const;
	void handleEvent(const sf::Event& event) const;

	void lateUpdate();

	void clearGuis();

private:
	void registerGui(GuiBase* guiBase);
	void disposeGui(const int id);
	void showGui(const int id);

	void switchInternal(const int id);
	void removeInternal(const int id);
};

