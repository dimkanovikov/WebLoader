#include "HttpMultiPart.h"
#include "QFreeDesktopMime/freedesktopmime.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>


HttpPart::HttpPart(HttpPartType _type) :
    m_type( _type )
{

}

HttpPart::HttpPartType HttpPart::type() const
{
	return m_type;
}

void HttpPart::setText(const QString _name, const QString _value)
{
    setName(_name);
    setValue(_value);
}

void HttpPart::setFile(const QString _name, const QString _filePath)
{
    setName(_name);
    setFilePath(_filePath);
}

QString HttpPart::name() const
{
	return m_name;
}

QString HttpPart::value() const
{
	return m_value;
}

QString HttpPart::fileName() const
{
	return value();
}

QString HttpPart::filePath() const
{
	return m_filePath;
}



void HttpPart::setName(const QString _name)
{
    if (m_name != _name)
        m_name = _name;
}

void HttpPart::setValue(const QString _value)
{
    if (m_value != _value)
        m_value = _value;
}

void HttpPart::setFileName(const QString _fileName)
{
    setValue(_fileName);
}

void HttpPart::setFilePath(const QString _filePath)
{
    if (m_filePath != _filePath) {
        m_filePath = _filePath;
        QString fileName = _filePath.split('/').last();
        setFileName(fileName);
	}
}





HttpMultiPart::HttpMultiPart()
{
}

void HttpMultiPart::setBoundary(const QString _boundary)
{
    if (m_boundary != _boundary) {
        m_boundary = _boundary;
    }
}

void HttpMultiPart::addPart(HttpPart _part)
{
    m_parts.append(_part);
}

QByteArray HttpMultiPart::data()
{
	QByteArray multiPartData;
	foreach ( HttpPart httpPart, parts() ) {
		QByteArray partData = makeDataFromPart( httpPart );
		multiPartData.append( partData );
	}
	// Добавление отметки о завершении данных
	{
		QByteArray endData = makeEndData();
		multiPartData.append( endData );
	}
	return multiPartData;
}

QByteArray HttpMultiPart::makeDataFromPart(HttpPart _part)
{
	QByteArray partData;
    switch (_part.type()) {
	case HttpPart::Text: {
        partData = makeDataFromTextPart( _part );
		break;
	}
	case HttpPart::File: {
        partData = makeDataFromFilePart( _part );
		break;
	}
	}
	return partData;
}

QByteArray HttpMultiPart::makeDataFromTextPart(HttpPart _part)
{
	QByteArray partData;

    partData.append("--");
    partData.append(boundary());
    partData.append(crlf());

	partData.append(
                QString("Content-Disposition: form-data; name=\"%1\"%3%3%2")
                .arg(_part.name(), _part.value(), crlf())
				);

    partData.append(crlf());

	return partData;
}

QByteArray HttpMultiPart::makeDataFromFilePart(HttpPart _part)
{
	QByteArray partData;

    partData.append("--");
    partData.append(boundary());
    partData.append(crlf());

	{
        QFile uploadFile(_part.filePath());
        uploadFile.open(QIODevice::ReadOnly);
		// Определение mime типа файла
		QFreeDesktopMime mimeTypeDetector;
        QString contentType = mimeTypeDetector.fromFile(&uploadFile);
        uploadFile.seek(0); // Несколько байт были считаны

		partData.append(
                    QString("Content-Disposition: form-data; name=\"%1\"; filename=\"%2\"%4"
							 "Content-Type: %3%4%4"
							 )
                    .arg(_part.name(),
                          _part.fileName(),
						  contentType,
                          crlf())
					);

        while (!uploadFile.atEnd()) {
			QByteArray readed = uploadFile.read(1024);
            partData.append(readed);
		}
		uploadFile.close();
	}

    partData.append(crlf());
	return partData;
}

QByteArray HttpMultiPart::makeEndData()
{
	QByteArray partData;

    partData.append("--");
	partData.append(boundary());
    partData.append("--");
    partData.append(crlf());

	return partData;
}

QString HttpMultiPart::boundary() const
{
	return m_boundary;
}

QString HttpMultiPart::crlf() const
{
	QString crlf;
	crlf = 0x0d;
	crlf += 0x0a;
	return crlf;
}

QList<HttpPart> HttpMultiPart::parts() const
{
	return m_parts;
}
