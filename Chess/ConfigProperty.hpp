#pragma once
#include <cctype>

inline int stob(const std::string& _Str)
{
	const char *_Ptr = _Str.c_str();

	if (!_Ptr)
		std::_Xinvalid_argument("invalid stob argument");

	std::string low(_Str);
	std::transform(low.begin(), low.end(), low.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);

	if (!low.compare("true")) return true;
	if (!low.compare("false")) return false;

	std::_Xinvalid_argument("stob argument out of range");
}

class ConfigProperty {
private:
	bool m_isRead;

protected:
	std::string m_name;
	std::string m_comment;

	using string = const std::string&;
	virtual void fromString(string value) = 0;
	virtual std::string getVerboseComment() = 0;

public:
	using Ptr = std::shared_ptr<ConfigProperty>;
	bool IsChanged;

	ConfigProperty(string name, string comment = "") :
		m_name(name),
		m_comment(comment),
		IsChanged(false),
		m_isRead(false)
	{ }
	virtual ~ConfigProperty() {}

	bool isRead() const {
		return m_isRead;
	}

	void readProperty(string value) {
		m_isRead = true;
		fromString(value);
	}
	virtual std::string valueToString() = 0;

	ConfigProperty& setComment(string comment) {
		m_comment = comment;
		return *this;
	}
	std::string getComment(bool isVerbose = true) {
		if (isVerbose)
			return m_comment + ' ' + getVerboseComment();
		return m_comment;
	}

	string getName() {
		return m_name;
	}
};

class StringProperty : public ConfigProperty {
	std::string m_defaultValue;
	std::string m_value;

	std::string getVerboseComment() override {
		return "[default: " + m_defaultValue + ']';
	}

	void fromString(string value) override {
		m_value = value;
	}

	std::string valueToString() override {
		return m_value;
	}

public:
	using Ptr = std::shared_ptr<StringProperty>;

	static Ptr create(string name, string defaultValue, string comment = "") {
		return std::make_shared<StringProperty>(name, defaultValue, comment);
	}

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
};

class BoolProperty : public ConfigProperty {
	bool m_defaultValue;
	bool m_value;

	std::string getVerboseComment() override {
		std::string defVal = m_value ? "true" : "false";
		return "[default: " + defVal + ']';
	}

	void fromString(const std::string & value) override {
		try {
			m_value = stob(value);
		}
		catch (...) {
			m_value = m_defaultValue;
		}
	}

	std::string valueToString() override {
		return m_value ? "true" : "false";
	}

public:
	using Ptr = std::shared_ptr<BoolProperty>;

	static Ptr create(string name, bool defaultValue, string comment = "") {
		return std::make_shared<BoolProperty>(name, defaultValue, comment);
	}

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
};

template<typename T, typename PropType>
class NumericProperty : public ConfigProperty {
	std::string getVerboseComment() override {
		T defaultMinValue = std::numeric_limits<T>::min();
		T defaultMaxValue = std::numeric_limits<T>::max();
		std::stringstream ss;
		ss << '[';
		if (m_minValue != defaultMinValue && m_maxValue != defaultMaxValue)
			ss << "range: " << m_minValue << " ~ " << m_maxValue << ", ";
		else {
			if (m_minValue != defaultMinValue)
				ss << "min: " << m_minValue << ", ";
			if (m_maxValue != defaultMaxValue)
				ss << "max: " << m_maxValue << ", ";
		}
		ss << "default: " << m_defaultValue << ']';
		return ss.str();
	}

	inline void innerCreate(T defaultValue, T minValue, T maxValue) {
		if (defaultValue < minValue)
			throw new std::invalid_argument("Defaultvalue is smaller than MinValue");

		if (defaultValue > maxValue)
			throw new std::invalid_argument("Defaultvalue is larger than MaxValue");

		if (minValue > maxValue)
			throw new std::invalid_argument("MinValue is larger than MaxValue");

		m_defaultValue = defaultValue;
		m_value = defaultValue;
		m_minValue = minValue;
		m_maxValue = maxValue;
	}

protected:
	T m_defaultValue;
	T m_value;
	T m_minValue;
	T m_maxValue;

public:
	NumericProperty(string name, T defaultValue, string comment = "") :
		ConfigProperty(name, comment) {
		innerCreate(defaultValue, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	}
	NumericProperty(string name, T defaultValue, T minValue, T maxValue, string comment = "") :
		ConfigProperty(name, comment) {
		innerCreate(defaultValue, minValue, maxValue);
	}

	static std::shared_ptr<PropType> create(string name, T defaultValue, string comment = "") {
		return std::make_shared<PropType>(name, defaultValue, comment);
	}
	static std::shared_ptr<PropType> create(string name, T defaultValue, T minValue, T maxValue, string comment = "") {
		return std::make_shared<PropType>(name, defaultValue, minValue, maxValue, comment);
	}

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

	void fromString(const std::string & value) override {
		try {
			setValue(std::stoi(value));
		}
		catch (...) {
			m_value = m_defaultValue;
		}
	}

	std::string valueToString() override {
		return std::to_string(m_value);
	}

public:
	using Ptr = std::shared_ptr<IntProperty>;
	using NumericProperty::NumericProperty;
};

class LongProperty : public NumericProperty<long long, LongProperty> {
	using Long = long long;

	void fromString(const std::string & value) override {
		try {
			setValue(std::stoll(value));
		}
		catch (...) {
			m_value = m_defaultValue;
		}
	}

	std::string valueToString() override {
		return std::to_string(m_value);
	}

public:
	using Ptr = std::shared_ptr<LongProperty>;
	using NumericProperty::NumericProperty;
};

class FloatProperty : public NumericProperty<float, FloatProperty> {
	void fromString(const std::string & value) override {
		try {
			setValue(std::stof(value));
		}
		catch (...) {
			m_value = m_defaultValue;
		}
	}

	std::string valueToString() override {
		return std::to_string(m_value);
	}

public:
	using Ptr = std::shared_ptr<FloatProperty>;
	using NumericProperty::NumericProperty;
};

class DoubleProperty : public NumericProperty<double, DoubleProperty> {
	void fromString(const std::string& value) override {
		try {
			setValue(std::stod(value));
		}
		catch (...) {
			m_value = m_defaultValue;
		}
	}

	std::string valueToString() override {
		return std::to_string(m_value);
	}

public:
	using Ptr = std::shared_ptr<DoubleProperty>;
	using NumericProperty::NumericProperty;
};