#pragma once
#include <QString>
#include <QDate>
class QXmlStreamAttributes;

class UnitMsg
{
public:
	enum class Source :std::uint8_t
	{//来源枚举
		CRAFT = 0,			//创意工坊
		LOCAL = 1,			//本地
		INTERNET = 2		//网络
	};

	UnitMsg(QXmlStreamAttributes* attributes);
	UnitMsg(QString strFileName, bool bHaveThumbnail = true);
	QString WriteName();
	QString WritePhotoPath();
	QString WriteVideoPath();
	QString WriteMportTime();
	QString WriteUserTime();
	QString WriteSource();
	QString WriteLove();
	QString GetName();
	QString GetPhotoPath();
	QString GetVideoPath();
	QDate GetMportTime();
	QDate GetUserTime();
	Source GetSource();
	bool SetName(QString strName);
	void SetPhoto(QString strPath);
	void SetVideo(QString strPath);
	void SetMporTime(QDate date);
	void SetUserTime(QDate date);
	void SetSource(Source source);
	inline void SetLove(bool bState)
	{
		if (bState)
		{
			m_cState |= 2;
		}
		else
		{
			m_cState &= 255 ^ 2;
		}
	}

	inline void SetVisual(bool bShow)
	{
		if (bShow)
		{
			m_cState &= 255 ^ 1;
		}
		else
		{
			m_cState |= 1;
		}
	}

	inline void Delete()
	{
		m_cState |= 4;
	}

	inline bool IsLove()
	{
		return m_cState & 2;
	}

	inline bool IsShow()
	{
		return !((m_cState & 1) && (m_cState & 4));
	}


private:
	QString m_strName;			//资源名字
	QString m_strPhoto;			//预览图路径
	QString m_strVideo;			//视频路径
	QDate m_dateMportTime;		//导入时间
	QDate m_dateUserTime;		//最后一次使用时间
	Source m_source;			//来源
	uchar m_cState;				//状态
	/*state说明
	*从右到左
	* 1.为1则隐藏，为0则没有隐藏
	* 2.为1则喜欢，为0为非喜欢
	* 3.为1删除状态，为0则没有删除
	*/

	bool ReName(QString strNewName);		//更改资源文件夹名字
};