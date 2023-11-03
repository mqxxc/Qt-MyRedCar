#pragma once
//宏定义文件
#define MainWnd_W 1200			//开发机中主窗口宽
#define MainWnd_H 930			//开发机种主窗口高

#ifdef ThemeWnd_H
#define Max_8bit 255				//uchar最大值
#define titleh 40					//标题栏高度
#define appIcoPath "/res/images/ico/33.png"	//应用图标路径
#define titleIcoPath "/res/images/ico/ico.png"	//标题栏图标路径
#define comboBox1 "名字"			//筛选框1
#define comboBox2 "添加时间 "		//筛选框2
#define comboBox3 "最后使用 "		//筛选框3
#define comboBox4 "喜欢 "			//筛选框4
#define inputTxt "搜索 "			//搜索框文本
#define inputIco "/res/images/ico/search.png"		//搜索框右侧图标
#define MsgTitle "嘤嘤嘤 "			//信息提示框标题
#define Msg_originality "抱歉，作者还在学爬虫，所以此功能暂未开放！ "		//浏览创意工坊文本提示
#define Msg_about_link "https://space.bilibili.com/224669104?spm_id_from=333.1007.0.0"	//关于我的链接
#define dialogTitle "选中文件"				//文本选择框标题栏
#define dialogFilter "视频文件(*.mp4)"		//文本选择框文件类型选择器
#define loadFinish_Text "导入成功！ "	//本地文件导入成功提示文本
#define loadFail "导入失败请稍后重试！"	//本地文件导入失败提示文本
#define Msg_filter "抱歉，资源尚未分类，预计与创意工坊一起开放！\n未来可期"	//资源筛选文本提示
#define sort_p "/res/images/ico/sort_p.png"			//排序正序的图标路径
#define sort_r "/res/images/ico/sort_r.png"			//排序倒叙的图标路径
#define settingIco "/res/images/ico/settingIco.png"	//设置按钮图标路径
#endif // ThemeWnd_H

//Elastic
#ifdef Elastic_H
#define mouse_sens 10					//鼠标对窗口边缘敏感程度
#define StateMax 255
#endif

//UnitMsg
#ifdef UnitMsg_H
#define dePhotoS ".jpg"					//缩略图默认后缀名
#define deVideoS ".mp4"					//默认视频后缀
#define dePhotoPath "defaultPhoto.jpg"	//默认图片名字
#define deusedTime "2022-05-04"			//默认最后一次使用时间
#define recordPath "/res/media/"		//本地存储文件路径
#endif


//Uint
#ifdef Unit_H
#define UnitWidth 120					//开发机推荐单元宽	
#define menuRN "重命名 "				//菜单项文本
#define menuNL "取消喜欢 "				//菜单项文本
#define menuIL "设为喜欢 "				//菜单项文本
#define menuDe "删除 "					//菜单项文本
#define datePath "/res/media/"			//资源文件中间路径
#endif

//RcWidget
#ifdef RcWidget_H
#define ratio 8							//控件与空隙的比
#define dataPath "/res/media/"			//资源文件中间路径
#define comboBox_onne "默认"			//筛选框0
#define comboBox_name "名字 "			//筛选框1
#define comboBox_iTime "添加时间 "		//筛选框2
#define comboBox_uTime "最后使用 "		//筛选框3
#define comboBox_love "喜欢"			//筛选框4
#define deVideoS ".mp4"					//默认视频后缀
#define dePhotoS ".jpg"					//缩略图默认后缀名
#define maxControlSig 255				//控制信最大值
#endif

//UnitMsgs_H
#ifdef  UnitMsgs_H
#define initTextPath "conf/dataMsg.xml"		//缓存文件路径
#endif