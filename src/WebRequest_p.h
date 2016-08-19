#ifndef WEBREQUEST_H
#define WEBREQUEST_H

#include <QString>
#include <QVariant>
#include <QNetworkRequest>


/*!
  \class WebLoader

  \brief Класс обертка для QNetworkRequest
  */
class WebRequest
{
public:
	/*!
	  \fn Конструктор
	  */
	WebRequest();
	~WebRequest();

	/*!
	  \fn Ссылка запроса
	  */
	QUrl urlToLoad() const;
	/*!
	  \fn Установка ссылки для запроса
	  \param url - ссылка
	  */
    void setUrlToLoad(QUrl _url);

	/*!
	  \fn Ссылка referer
	  */
	QUrl urlReferer() const;
	/*!
	  \fn Установка ссылки referer'а
	  \param url - ссылка
	  */
    void setUrlReferer(QUrl _url);
	/*!
	 * \fn Очистить список атрибутов
	 */
	void clearAttributes();
	/*!
	  \fn Добавление текстового атрибута в запрос
	  \param name - название атрибута
	  \param value - значение атрибута
	  */
    void addAttribute(QString _name, QVariant _value);
	/*!
	  \fn Добавление атрибута-файла в запрос
	  \param name - название атрибута
	  \param filePath - путь к файлу
	  */
    void addAttributeFile(QString _name, QString _filePath);

	/*!
	  \fn Сформированный объект класса QNetworkRequest
	  */
    QNetworkRequest networkRequest(bool _addContentHeaders = false);

	/*!
	  \fn Атрибуты запроса
	  */

	QByteArray  multiPartData();

//*****************************************************************************
// Внутренняя реализация класса

private:
	/*!
	  \fn Текстовые атрибуты запроса
	  */
	QList< QPair< QString, QVariant > > attributes() const;
	/*!
	  \fn Добавление текстового атрибута в запрос
	  \param attribute - имя + значения атрибута
	  */
    void addAttribute(QPair< QString, QVariant > _attribute);
	/*!
	  \fn Атрибуты-файлы запроса
	  */
	QList<QPair<QString, QString> > attributeFiles() const;
	/*!
	  \fn Добавление атрибута-файла в запрос
	  \param attributeFile - имя атрибута + путь к файлу
	  */
    void addAttributeFile(QPair<QString, QString> _attributeFile);

private:
	QUrl m_urlToLoad,
		 m_urlReferer;
	QList< QPair< QString, QVariant > > m_attributes;
	QList< QPair< QString, QString > >  m_attributeFiles;
};

#endif // WEBREQUEST_H
