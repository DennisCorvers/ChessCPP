#pragma once
struct ConfigProperty {
private:
	std::string m_name;
	std::string m_comment;

	std::string m_minValue;
	std::string m_maxValue;
	std::string m_defaultValue;
	std::string m_value;

	bool m_isChanged;

	inline void setMinValue(const std::string& minValue) {
		m_minValue = minValue;
	}
	inline void setMaxValue(const std::string& maxValue) {
		m_maxValue = maxValue;
	}

	using string = const std::string&;

public:
	ConfigProperty(string name, string value, string comment = "") :
		m_name(name),
		m_value(value),
		m_defaultValue(value),
		m_comment(comment),
		m_minValue("-2147483647"),
		m_maxValue("2147483647"),
		m_isChanged(false)
	{ }

	virtual ~ConfigProperty() {};

	void setToDefault() {
		m_value = m_defaultValue;
	}

	void resetChangedState() {
		m_isChanged = false;
	}

	const std::string& getComment() {
		return m_comment;
	}
	const std::string& getName() {
		return m_name;
	}
	const std::string& getValue() {
		return m_value;
	}
	const std::string& getDefaultValue() {
		return m_defaultValue;
	}
	const std::string& getMinValue() {
		return m_minValue;
	}
	const std::string& getMaxValue() {
		return m_maxValue;
	}

	bool isBoolValue() {
		std::string low = String::toLower(m_value);
		return low.compare("true") || low.compare("false");
	}
	bool getBool() {
		return isBoolValue() ? String::stob(m_value) : String::stob(m_defaultValue);
	}
	bool getBool(bool defaultValue) {
		return isBoolValue() ? String::stob(m_value) : defaultValue;
	}

	int getInt() {
		return getInt(std::stoi(m_defaultValue));
	}
	int getInt(int defaultValue) {
		try {
			std::stoi(m_value);
		}
		catch (...) {
			return defaultValue;
		}
	}

	long getLong() {
		return getLong(std::stol(m_defaultValue));
	}
	long getLong(long defaultValue) {
		try {
			std::stol(m_value);
		}
		catch (...) {
			return defaultValue;
		}
	}

	float getFloat() {
		return getFloat(std::stof(m_defaultValue));
	}
	float getFloat(float defaultValue) {
		try {
			std::stof(m_value);
		}
		catch (...) {
			return defaultValue;
		}
	}

	double getDouble() {
		return getDouble(std::stod(m_defaultValue));
	}
	double getDouble(double defaultValue) {
		try {
			std::stod(m_value);
		}
		catch (...) {
			return defaultValue;
		}
	}

	inline void setValue(const std::string& value) {
		m_value = value;
		m_isChanged = true;
	}
	void setValue(bool value) {
		if (value)
			setValue("true");
		else
			setValue("false");
	}
	void setValue(int value) {
		setValue(std::to_string(value));
	}
	void setValue(long value) {
		setValue(std::to_string(value));
	}
	void setValue(float value) {
		setValue(std::to_string(value));
	}
	void setValue(double value) {
		setValue(std::to_string(value));
	}

	inline void setDefaultValue(const std::string& value) {
		m_defaultValue = value;
	}
	void setDefaultValue(bool value) {
		if (value)
			setDefaultValue("true");
		else
			setDefaultValue("false");
	}
	void setDefaultValue(int value) {
		setDefaultValue(std::to_string(value));
	}
	void setDefaultValue(long value) {
		setDefaultValue(std::to_string(value));
	}
	void setDefaultValue(float value) {
		setDefaultValue(std::to_string(value));
	}
	void setDefaultValue(double value) {
		setDefaultValue(std::to_string(value));
	}

	void setMinValue(int value) {
		setMinValue(std::to_string(value));
	}
	void setMinValue(long value) {
		setMinValue(std::to_string(value));
	}
	void setMinValue(float value) {
		setMinValue(std::to_string(value));
	}
	void setMinValue(double value) {
		setMinValue(std::to_string(value));
	}

	void setMaxValue(int value) {
		setMaxValue(std::to_string(value));
	}
	void setMaxValue(long value) {
		setMaxValue(std::to_string(value));
	}
	void setMaxValue(float value) {
		setMaxValue(std::to_string(value));
	}
	void setMaxValue(double value) {
		setMaxValue(std::to_string(value));
	}
};