#ifndef HTTPMULTIPART_H
#define HTTPMULTIPART_H

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QList>

class HttpPart
{
public:
	enum HttpPartType {
		Text,
		File
	};

public:
    HttpPart(HttpPartType _type = Text);
	HttpPartType type() const;
    void setText(const QString _name, const QString _value);
    void setFile(const QString _name, const QString _filePath);

public:
	QString name() const;
	QString value() const;
	QString fileName() const;
	QString filePath() const;


private:
    void setName(const QString _name);
    void setValue(const QString _value);
    void setFileName(const QString _fileName);
    void setFilePath(const QString _filePath);

private:
	HttpPartType m_type;
	QString m_name,
			m_value,
			m_filePath;
};

class HttpMultiPart
{
public:
	HttpMultiPart();
    void setBoundary(const QString _boundary);
    void addPart(HttpPart _part);

	QByteArray data();

private:
    QByteArray makeDataFromPart(HttpPart _part);
    QByteArray makeDataFromTextPart(HttpPart _part);
    QByteArray makeDataFromFilePart(HttpPart _part);
	QByteArray makeEndData();

private:
	QString boundary() const;
	QString crlf() const;
	QList<HttpPart> parts() const;

private:
	QString m_boundary;
	QList<HttpPart> m_parts;
};

#endif // HTTPMULTIPART_H
