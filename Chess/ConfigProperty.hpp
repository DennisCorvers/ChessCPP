#pragma once
class ConfigProperty {
private:
	std::string m_name;
	std::string m_comment;

	bool m_isRead;

protected:
	using string = const std::string&;
	virtual void fromString(string value) = 0;

public:
	using Ptr = std::shared_ptr<ConfigProperty>;
	bool IsChanged;

	ConfigProperty(string name, string comment = "") :
		m_name(name),
		m_comment(comment),
		IsChanged(false),
		m_isRead(false)
	{ }

	bool isRead() const {
		return m_isRead;
	}

	void readProperty(string value) {
		m_isRead = true;
		fromString(value);
	}

	ConfigProperty& setComment(string comment) {
		m_comment = comment;
		return *this;
	}

	string getComment() {
		return m_comment;
	}

	string getName() {
		return m_name;
	}
};

class StringProperty : public ConfigProperty {
private:
	std::string m_defaultValue;
	std::string m_value;

public:
	StringProperty(string name, string defaultValue, string comment = "") :
		m_defaultValue(defaultValue),
		m_value(defaultValue),
		ConfigProperty(name, comment) {	}

	string getValue() {
		return m_value;
	}

	StringProperty& setValue(string value) {
		IsChanged = true;
		m_value = value;
	}

	virtual void fromString(string value) override {
		m_value = value;
	}
};

class BoolProperty : public ConfigProperty {
private:
	bool m_defaultValue;
	bool m_value;

public:
	BoolProperty(string name, bool defaultValue, string comment = "") :
		m_defaultValue(defaultValue),
		m_value(defaultValue),
		ConfigProperty(name, comment) { }

	bool getValue() {
		return m_value;
	}

	BoolProperty& setValue(bool value) {
		IsChanged = true;
		m_value = value;
	}

	virtual void fromString(const std::string & value) override {
		try {
			m_value = String::stob(value);
		}
		catch (const std::invalid_argument&) {
			m_value = m_defaultValue;
		}
	}
};

template<typename T, typename PropType>
class NumericProperty : public ConfigProperty {
public:
	T m_defaultValue;
	T m_value;
	T m_minValue;
	T m_maxValue;

protected:
	NumericProperty(string name, T defaultValue, T minValue, T maxValue, string comment = "") :
		m_value(defaultValue),
		m_defaultValue(defaultValue),
		m_minValue(minValue),
		m_maxValue(maxValue),
		ConfigProperty(name, comment)
	{
		if (defaultValue < minValue)
			throw new std::invalid_argument("Defaultvalue is smaller than MinValue");

		if (defaultValue > maxValue)
			throw new std::invalid_argument("Defaultvalue is larger than MaxValue");

		if (minValue > maxValue)
			throw new std::invalid_argument("MinValue is larger than MaxValue");
	}

public:
	PropType& setMinValue(T minValue) {
		m_minValue = minValue;
		return static_cast<PropType&>(*this);
	}
	const T getMinValue() const {
		return m_minValue;
	}

	PropType& setMaxValue(T maxValue) {
		m_maxValue = maxValue;
		return static_cast<PropType&>(*this);
	}
	const T getMaxValue() const {
		return m_maxValue;
	}

	const T getDefaultValue() const {
		return m_defaultValue;
	}

	PropType& setValue(T value) {
		m_value = value < m_minValue ? m_minValue : value > m_maxValue ? m_maxValue : value;
		return static_cast<PropType&>(*this);
	}
	const T getValue() const {
		return m_value;
	}
};

class IntProperty : public NumericProperty<int, IntProperty> {
public:
	IntProperty(string name, int defaultValue, string comment = "") :
		NumericProperty(name, defaultValue, INT_MIN, INT_MAX, comment) { }

	IntProperty(string name, int defaultValue, int minValue, int maxValue, string comment = "") :
		NumericProperty(name, defaultValue, minValue, maxValue, comment) { }

	virtual void fromString(const std::string & value) override {
		try {
			setValue(std::stoi(value));
		}
		catch (const std::invalid_argument&) {
			m_value = m_defaultValue;
		}
	}
};

//class LongProperty : public ConfigProperty {
//
//};
//
//class FloatProperty : public ConfigProperty {
//
//};
//
//class DoubleProperty : public ConfigProperty {
//
//};