// ListData.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "ListData.h"
#include "CError.h"

#include "functions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListData

CListData::CListData() : count(0)
{
	Graphic = new CListData::Graph(this);
}

CListData::~CListData()
{
	delete Graphic;
}

int CListData::AddData(double x, double y)
{
	CString temp;

	XDatasCache.push_back(x);
	YDatasCache.push_back(y);

	if (x>XMax) XMax=x;
	if (x<XMin) XMin=x;
	if (y>YMax) YMax=y;
	if (y<YMin) YMin=y;

	temp.Format("%d", count);
	InsertItem(count, temp);

// Insert le x
	temp.Format("%g", x);
	SetItem(count, 1, LVIF_TEXT , temp, NULL, NULL, NULL, NULL);

// Insert le y
	temp.Format("%g", y);
	SetItem(count, 2, LVIF_TEXT , temp, NULL, NULL, NULL, NULL);

	EnsureVisible(count,FALSE);
	count++;

	return count;
}

BEGIN_MESSAGE_MAP(CListData, CListCtrl)
	//{{AFX_MSG_MAP(CListData)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomdraw )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListData message handlers

BOOL CListData::DeleteAllItems()
{
	ListView_SetExtendedListViewStyleEx(GetSafeHwnd(), 
        LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	XDatasCache.clear();
	YDatasCache.clear();
	count = 0;
	XMin=1500;
	XMax=0;
	YMin=1500;
	YMax=0;
	return CListCtrl::DeleteAllItems();
}

BOOL CListData::DeleteItem(int nItem)
{
	count--;
	XDatasCache.erase(XDatasCache.begin()+nItem);
	YDatasCache.erase(YDatasCache.begin()+nItem);
	return CListCtrl::DeleteItem(nItem);
}

void CListData::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( GetFocus() != this) 
		SetFocus();

	LVHITTESTINFO ht;
	ht.pt = point;
	SubItemHitTest(&ht);
	
	CRect rc;
	GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
	InvalidateRect(rc);
	UpdateWindow();
	EnsureVisible(ht.iItem, FALSE);
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

// Sauvegarder les données de la liste sur le disque dur pour utilisation future.
bool CListData::SaveDataToDisk(const CString path)
{
	CString *data = new CString[count];
	HANDLE file = CreateFile(path,
		                     GENERIC_WRITE,
							 NULL,
                             NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);
	
	if(file == INVALID_HANDLE_VALUE)
	{
		ThrowError();
		return false;
	}

	unsigned long bytewritten;

	int i,j;
	for(i=0; i<count; i++)
	{
		for(j=1; j<=2; j++)
		{
			if(j==2)
			{
				data[i] += GetItemText(i, j);
				data[i] += "\r\n";
			}
			else
			{
				data[i] += GetItemText(i, j);
				data[i] += ',';
			}
		}
	}

	for(i=0; i<count; i++)
	{
		char* temp = data[i].GetBuffer(0);
		WriteFile(file, temp , data[i].GetLength(), &bytewritten, NULL);
	}

	char tag[15] = "<spectronic>";
	WriteFile(file, tag , strlen(tag), &bytewritten, NULL);

	CloseHandle(file);
	delete[] data;

	return true;
}

bool CListData::LoadDataFromDisk(const CString path)
{
	// Détruit tous les items dans la liste.
	DeleteAllItems();

	HANDLE file = CreateFile(path,
		                     GENERIC_READ,
							 NULL,
                             NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		// Le fichier est invalide
		ThrowError();
		return false;
	}
	
	int i=0,j=0;
	unsigned long byteread;
	char separator;		// Contient le spérateur des données texte

	int filesize = GetFileSize(file, NULL);

	char* buffer = new char[filesize];
	ReadFile(file, buffer, filesize, &byteread, NULL);

	string strbuf = buffer;
	delete[] buffer;

	string::iterator pos, end;
	end = strbuf.end();
	char tmpchar;

	// Trouve le séparateur de données entre la longueur d'onde et l'absorbance.
	for(pos=strbuf.begin(); pos<end; pos++)
	{
		tmpchar = pos[0];
		if(tmpchar == ',' || tmpchar == '\t' || tmpchar == ';' || tmpchar == ' ')
		{
			separator = tmpchar;
			break;
		}
	}

	// Compte le nombre de lignes dans le fichier.
	int line = 0;
	while(i<=filesize)
	{
		if(strbuf[i] == separator)
			line++;
		i++;
	}

	// Tente de trouvé l'identificateur de fichier spectronic dans le texte.
	int position;
	position = strbuf.find("<spectronic>");

	if(line == 0 || position == 0)
	{
		// Si le fichier ne contient pas de données ou si le fichier n'a pas l'identificateur
		// nous indiquons à l'utilisateur que le format du fichier n'est pas correct.  Ceci est
		// fait pour des raisons de sécurité et de stabilité du logiciel.
		ThrowError("Le format du fichier est incorrect ou le fichier ne contient pas de données");
		return false;
	}

	double datax=0;
	double datax2=0;
	double datay=0;
	string temp;
	i=0;
	j=0;

	// Extrais toutes les données du fichier et les mets dans la liste à l'écran.
	while(i<line)
	{
		while(strbuf[j] != '\r')
			temp +=strbuf[j++];
		// Ignorer l'identificateur de fichier si on le rencontre.
		if(temp == "<spectronic>")
		{
			j+=2;
			temp = "";
			continue;
		}

		j -= temp.length();
		temp = "";
		while(strbuf[j] != separator)
			temp+=strbuf[j++];
		j++;
		datax2 = datax;
		datax = atof(temp.c_str());
		temp = "";
		while(strbuf[j] != '\r')
			temp +=strbuf[j++];
		j+=2;
		datay = atof(temp.c_str());
		temp = "";
		AddData(datax, datay);
		i++;
	}

	double WvInc;

	if (datax>datax2)
		WvInc=(datax-datax2);
	else
		WvInc=(datax2-datax);

	//Arrondi WvInc au dixième
	if (WvInc-(((int)(WvInc/.1))/10)<.05)
		WvInc=(float)((int)(WvInc/.1))/10;
	else
		WvInc=(float)((int)(WvInc/.1)+1)/10;
	SetDataConf(false,WvInc);

	CloseHandle(file);
	return true;
}

void CListData::OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;
}

const map<double,double> CListData::ListToMap()
{
	map<double,double> list;

	double x;

	int i;
	string temp;
	for(i=0; i<count; i++)
	{
		x = atof(GetItemText(i, 1));
		list[x] = atof(GetItemText(i, 2));
	}

	return list;
}

const list<double> CListData::FileToList(const CString path)
{
	list<double> liste;

	HANDLE file = CreateFile(	path,
								GENERIC_READ,
								NULL,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		ThrowError();
		return liste;
	}
	
	unsigned long byteread;

	int length=0;
	int tempcount=0;

	int i;
	int pos=0;
	char* temp;
	for(i=0; i<6; i++)
	{
		ReadFile(file, &length, sizeof(int), &byteread, NULL);
		pos += sizeof(int);
		pos += length;
		temp = new char[length];
		ReadFile(file, temp, length, &byteread, NULL);
		delete[] temp;
	}

	SetFilePointer(file, pos, NULL, FILE_BEGIN);

	double temptemp;
	ReadFile(file, &tempcount, sizeof(int), &byteread, NULL);
	ReadFile(file, &temptemp, sizeof(double), &byteread, NULL);
	ReadFile(file, &temptemp, sizeof(double), &byteread, NULL);
	ReadFile(file, &temptemp, sizeof(double), &byteread, NULL);
	ReadFile(file, &temptemp, sizeof(double), &byteread, NULL);

	double tempdata;
	double x,y;
	int j;
	for(i=0; i<tempcount; i++)
	{
		for(j=0; j<2; j++)
		{
			ReadFile(file, &tempdata, sizeof(double), &byteread, NULL);
			if(j==0)
				x = tempdata;
			else
				y = tempdata;
		}
		liste.push_back(y);
	}

	CloseHandle(file);
	return liste;
}


void CListData::GiveMaxDlgInstance(CMaxDlg* MaxList)
{
	Graphic->GiveMaxDlgInstance(MaxList);
}


int CListData::GetCount()
{
	return count;
}

double CListData::GetXMin()
{
	return XMin;
}

double CListData::GetXMax()
{
	return XMax;
}

double CListData::GetYMin()
{
	return YMin;
}

double CListData::GetYMax()
{
	return YMax;
}

double CListData::GetXData(int Pos)
{
	return XDatasCache[Pos];
}

double CListData::GetYData(int Pos)
{
	return YDatasCache[Pos];
}

double CListData::GetInterval()
{
	return Interval;
}

void CListData::SetDataConf(BOOL Etal, double WVInc)
{
	Etalon=Etal;
	Interval=WVInc;
}

// Sauvegarde les données de la liste sur le disque dur et les données relatives au spectre lui-même.
bool CListData::SaveDataBinToDisk(const CString path, FileInfo info)
{
	HANDLE file = CreateFile(path,
		                     GENERIC_WRITE,
							 NULL,
                             NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);
	
	if(file == INVALID_HANDLE_VALUE)
	{
		ThrowError();
		return false;
	}

	unsigned long bytewritten;

	if(GetCount() >= 2)
	{
		double test = (GetXData(1) - GetXData(0)) - info.WvInc;
		if(test <=-0.01 || test>=0.01)
			ThrowError("Votre incrémentation n'est peut etre pas valide.  La prochaine fois que vous\r\nutiliserez se graphique, peut-être que l'édition de graphique ne fonctionnera pas.");
	}

	// Sauvegarde toutes les données concernant le spectre.
	int length = info.Etalon.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Etalon.GetBuffer(0), length, &bytewritten, NULL);

	length = info.Title.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Title.GetBuffer(0), length, &bytewritten, NULL);

	length = info.Subst.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Subst.GetBuffer(0), length, &bytewritten, NULL);

	length = info.Name.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Name.GetBuffer(0), length, &bytewritten, NULL);

	length = info.Group.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Group.GetBuffer(0), length, &bytewritten, NULL);

	length = info.Date.GetLength() + 1;
	WriteFile(file, &length, sizeof(int), &bytewritten, NULL);
	WriteFile(file, info.Date.GetBuffer(0), length, &bytewritten, NULL);

	WriteFile(file, &count, sizeof(int), &bytewritten, NULL);
	WriteFile(file, &info.WvInc, sizeof(double), &bytewritten, NULL);
	WriteFile(file, &info.Speed, sizeof(double), &bytewritten, NULL);
	WriteFile(file, &info.StrtWv, sizeof(double), &bytewritten, NULL);
	WriteFile(file, &info.StopWv, sizeof(double), &bytewritten, NULL);

	// Sauvegarde les données de la liste.
	double data;
	int i,j;
	for(i=0; i<count; i++)
	{
		for(j=1; j<3; j++)
		{
			data = atof(GetItemText(i,j));
			WriteFile(file, &data, sizeof(double), &bytewritten, NULL);
		}
	}

	CloseHandle(file);

	return true;
}

FileInfo CListData::LoadDataBinFromDisk(const CString path)
{
	DeleteAllItems();

	FileInfo info;

	HANDLE file = CreateFile(path,
		                     GENERIC_READ,
							 NULL,
                             NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		ThrowError();
		info.WvInc = 0;
		return info;
	}
	
	unsigned long byteread;

	int size = GetFileSize(file, NULL);

	if(size < (7*sizeof(int)+4*sizeof(double)))
	{
		ThrowError("Fichier corrompu ou invalide");
		info.WvInc = 0;
		return info;
	}

	int length=0;
	int tempcount;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	char* temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Etalon = temp;
	delete[] temp;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Title = temp;
	delete[] temp;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Subst = temp;
	delete[] temp;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Name = temp;
	delete[] temp;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Group = temp;
	delete[] temp;

	ReadFile(file, &length, sizeof(int), &byteread, NULL);
	temp = new char[length];
	ReadFile(file, temp, length, &byteread, NULL);
	info.Date = temp;
	delete[] temp;

	ReadFile(file, &tempcount, sizeof(int), &byteread, NULL);
	ReadFile(file, &info.WvInc, sizeof(double), &byteread, NULL);
	ReadFile(file, &info.Speed, sizeof(double), &byteread, NULL);
	ReadFile(file, &info.StrtWv, sizeof(double), &byteread, NULL);
	ReadFile(file, &info.StopWv, sizeof(double), &byteread, NULL);

	if(tempcount > 100000)
	{
		ThrowError("Fichier corrompu ou invalide");
		info.WvInc = 0;
		return info;
	}

	Interval = info.WvInc;

	double tempdata;
	double x,y;
	int i,j;
	for(i=0; i<tempcount; i++)
	{
		for(j=0; j<2; j++)
		{
			ReadFile(file, &tempdata, sizeof(double), &byteread, NULL);
			if(j==0)
				x = tempdata;
			else
				y = tempdata;
		}
		AddData(x,y);
	}

	CloseHandle(file);
	return info;
}

void CListData::DrawGraph(CRect *Area, float XGraduation, int XGraduationLabel, float YGraduation, 
					   int YGraduationLabel, CDC *DestDC, BOOL Print)
{
	if(count>0)
	{
		XGraduationLabel=(int)((float)XGraduationLabel/XGraduation);
		YGraduationLabel=(int)((float)YGraduationLabel/YGraduation);
		Graphic->DisplayGraph(	Area, XGraduation,
								XGraduationLabel, YGraduation, 
								YGraduationLabel, DestDC, Print);
	}
}

void CListData::Graph::GraphBitmap::SaveBitmap(char* path)
{
	HANDLE File;
	BITMAPFILEHEADER FHeader;
	unsigned long ByteWritten;

	FHeader.bfType='BM';
	FHeader.bfSize=sizeof(FHeader)+
		sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*2+
		Bitmap.ScanSize*Bitmap.ScanNum;
	FHeader.bfReserved1=0;
	FHeader.bfReserved2=0;
	FHeader.bfOffBits=sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*2;

	File=CreateFile(path,GENERIC_READ|GENERIC_WRITE,
					0, 0, CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL,0);
	WriteFile(File,&FHeader,sizeof(FHeader),
		&ByteWritten,0);
	WriteFile(File,&Bitmap.BmpInfo.bmiHeader,
		sizeof(Bitmap.BmpInfo.bmiHeader),
		&ByteWritten,0);
	WriteFile(File,&Bitmap.BmpInfo.bmiColors,
		sizeof(RGBQUAD)*2,&ByteWritten,0);
	WriteFile(File,Bitmap.DIB,
		Bitmap.ScanNum*Bitmap.ScanSize,
		&ByteWritten, 0);
	CloseHandle(File);
}
/*
CListData::Graph::GraphBitmap::DisplayGraph(float XGraduation, int XGraduationLabel, float YGraduation, 
								   int YGraduationLabel)
{
	double XMin=DataList->GetXMin();
	double XMax=DataList->GetXMax();
	double YMin=DataList->GetYMin();
	double YMax=DataList->GetYMax();

	double Width=(XMax-XMin);
	double Height=(YMax-YMin);


	int Count=DataList->GetCount();
	double Interval=DataList->GetInterval();//DataList->GetInterval();

	double XScale=Width/(Area->right-Area->left);
	double YScale=Height/(Area->bottom-Area->top);

	Brush.CreateSolidBrush(GetSysColor(COLOR_MENU));	//Une brush blanche est necessaire pour dessiner le rectangle
														//blanc du graphique
	pen.CreatePen(PS_SOLID, 1, RGB(192,0,0));
	DestDC->FillRect(Area, &Brush);

	DrawAxes(	Area, XMin, XMax, YMin, YMax, XGraduation,
				XGraduationLabel, YGraduation, YGraduationLabel, DestDC);

	DestDC->SelectObject(pen);
	for (int i = 0; i<(Count-1); i++)
	{

		double YFrom=DataList->GetYData(i);
		double YTo=DataList->GetYData(i+1);
		double XFrom=DataList->GetXData(i);
		double XTo=DataList->GetXData(i+1);

		double x1=Area->left+(XFrom-XMin)/XScale;
		double y1=(Area->bottom)-(YFrom-YMin)/YScale;
		double x2=Area->left+(XTo-XMin)/XScale;
		double y2=(Area->bottom)-(YTo-YMin)/YScale;
		DrawLine(x1, y1, x2 ,y2, DestDC);
	}
}

CListData::Graph::GraphBitmap::DrawAxes(CRect *Area, double XMin, double XMax, double YMin, double YMax,
								double XGraduation, int XGraduationLabel, double YGraduation, 
								int YGraduationLabel, CDC *DestDC)
{
	double XScale=(XMax-XMin)/(Area->right-Area->left);
	double YScale=(YMax-YMin)/(Area->bottom-Area->top);
	
	if (YMin>=0)
	{
		DrawLine(Area->left,Area->bottom,Area->right,Area->bottom,DestDC);
	}
	else
	{
		DrawLine(Area->left,Area->bottom+(YMin/YScale),Area->right,Area->bottom+(YMin/YScale),DestDC);
	}
	if (XGraduation)
		if ((YMin/YScale)<=-5)
			for(double i=Area->left;i<Area->right;i+=(XGraduation/XScale))
				DrawLine(i,Area->bottom+(YMin/YScale)+5,i,Area->bottom+(YMin/YScale)-5,DestDC);
		else
			for(double i=Area->left;i<Area->right;i+=(XGraduation/XScale))
				DrawLine(i,Area->bottom,i,Area->bottom-5,DestDC);
	if (XMin>=0)
	{
		DrawLine(Area->left,Area->bottom,Area->left,Area->top,DestDC);
	}
	else
	{
		DrawLine(Area->left-(XMin/XScale),Area->bottom,Area->left-(XMin/XScale),Area->top,DestDC);
	}
	if (YGraduation)
		if ((XMin/XScale)<=-5)
			for(double i=Area->bottom;i>Area->top;i-=(YGraduation/YScale))
				DrawLine(Area->left-(XMin/XScale)+5,i,Area->left-(XMin/XScale)-5,i,DestDC);
		else
			for(double i=Area->bottom;i>Area->top;i-=(YGraduation/YScale))
				DrawLine(Area->left,i,Area->left+5,i,DestDC);
	//On met les graduations que si XGraduation
	//n'est pas nulle, si c nulle, ca veut que
	//l'on veut pas graduer l'axe des abcisses.
}
*/

CListData::Graph::GraphBitmap::GraphBitmap(int Width, int Height, HDC DestDC)
{
	int LineSize;
	int BmpSize;
//Les avertissements sur les booleans forcés ne
//sont pas pertinents ici, car il nous allons
//les utiliser ici pour optimiser et éviter
//de mettre plein de if

	#pragma warning(disable:4800)
//Si Width = 0, bool(Width)=false=0
//Si Width !=0, bool(Width)=true=1
//Il y a 8 points par byte.  Les points qui restent
//apres la division prennent un byte de plus
//Ensuite, il faut ajouter des bytes nul pour
//que le bitmap soit un entier de 4

//L'algorithme suivant divise la largeur(en pixels)
//par 8, et si il y a un reste, il ajoute 1 au
//résultats.  Ensuite, il verifie que la valeur
//obtenue soit un multiple de 4, sinon, il 
//l'arondie au prochain multiple de 4.

//exemple. si Width=12, la division par 8 donne 1
//reste 4. Puisque le reste != 0, il ajoute 1.
//Donc, nous avons la valeur 2.  C'est le nombre
//de bytes requis pour 12 pixels.  Mais nous devons
//maintenant arrondir ce nombre au prochain
//multiple de 4, puisque 2 n'en est pas un.
//Ensuite, on divise 2 par 4.  Nous obtenons 0.
//Puisque le reste de 2/4 n'est pas 0, nous
//rajoutons 1. Nous sommes donc rendu avec 1.
//Ensuite, on multiplie cette valeur par 4.
//L'algorithme retourne donc la valeur 4.
//La valeur que notre programme donne est 639 et
//l'algorithme retourne 80, donc ça marche

	LineSize=((((Width/8)+bool(Width % 8))/4)+1*
		((bool)(((Width/8)+bool(Width % 8))%4)))*4;
	BmpSize=LineSize*Height;
	Bitmap.DIB = new char[BmpSize];
	for (int i=0;i<BmpSize;i++)
		Bitmap.DIB[i]=0;
	//Bitmap.BmpInfo et Color2 font partie de la
	//structure servant à décrire le bitmap auprès
	//des API de Windows.
	Bitmap.BmpInfo.bmiHeader.biSize=
		sizeof(Bitmap.BmpInfo)-4;
	Bitmap.BmpInfo.bmiHeader.biWidth=Width;
	Bitmap.BmpInfo.bmiHeader.biHeight=Height;
	Bitmap.BmpInfo.bmiHeader.biPlanes=1;
	Bitmap.BmpInfo.bmiHeader.biBitCount=1;
	Bitmap.BmpInfo.bmiHeader.biCompression=BI_RGB;
	//biSizeImage n'est pas utile sans compression
	Bitmap.BmpInfo.bmiHeader.biSizeImage=0;
	Bitmap.BmpInfo.bmiHeader.biXPelsPerMeter=
		((float)1000*(float)::GetDeviceCaps(DestDC,HORZRES)/
		(float)::GetDeviceCaps(DestDC,HORZSIZE));
	Bitmap.BmpInfo.bmiHeader.biYPelsPerMeter=
		((float)1000*(float)::GetDeviceCaps(DestDC,VERTRES)/
		(float)::GetDeviceCaps(DestDC,VERTSIZE));
	Bitmap.BmpInfo.bmiHeader.biClrUsed=0;
	Bitmap.BmpInfo.bmiHeader.biClrImportant=0;
	Bitmap.ScanNum=Height;
	Bitmap.ScanSize=LineSize;

	Bitmap.BmpInfo.bmiColors[0].rgbBlue=255;
	Bitmap.BmpInfo.bmiColors[0].rgbRed=255;
	Bitmap.BmpInfo.bmiColors[0].rgbGreen=255;
	Bitmap.BmpInfo.bmiColors[0].rgbReserved=0;
	Bitmap.BmpInfo.bmiColors[1].rgbBlue=0;
	Bitmap.BmpInfo.bmiColors[1].rgbRed=0;
	Bitmap.BmpInfo.bmiColors[1].rgbGreen=0;
	Bitmap.BmpInfo.bmiColors[1].rgbReserved=0;
}

CListData::Graph::GraphBitmap::~GraphBitmap()
{
	delete Bitmap.DIB;
}

void CListData::Graph::GraphBitmap::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	float DeltaX;
	float DeltaY;
	unsigned int length;
	unsigned int i;

	if (!(y1*Bitmap.ScanSize+(unsigned int)(x1/8)>Bitmap.ScanNum*Bitmap.ScanSize)||
	   (y2*Bitmap.ScanSize+(unsigned int)(x2/8)>Bitmap.ScanNum*Bitmap.ScanSize))
	{
		if (x2==x1)
			if (y2>y1)
				for(i=y1;i<=y2;i++)
					PutPixel(x1,i);
			else
				for(i=y2;i<=y1;i++)
					PutPixel(x1,i);
		else
		{
			length=sqrt(((int)y2-(int)y1)*((int)y2-(int)y1)+((int)x2-(int)x1)*((int)x2-(int)x1));
			DeltaX=(float)((int)x2-(int)x1)/length;
			DeltaY=(float)((int)y2-(int)y1)/length;
			// {DeltaX,DeltaY} est le vecteur unitaire
			// directeur de la droite.
			// Sa fonction paramétrique est :
			// x=x1+DeltaX*i             y=y1+DeltaY*i
			for(i=0;i<=length;i++)
				PutPixel((unsigned int)(DeltaX*(float)i+(float)x1),(unsigned int)(DeltaY*(float)i+(float)y1));
			PutPixel(x2,y2);
		}
	}
}

void CListData::Graph::GraphBitmap::PutPixel(unsigned int x, unsigned int y)
{
	int ScanLine;

	//y*Bitmap.ScanSize+(int)(x/8) ne doit pas
	//dépasser la grosseur du tableau Bitmap.DIB
	//PutPixel ne verifie pas, car PutPixel est une
	//fonction très occupé, donc qui doit être le
	//plus vite que possible.  DrawLine sera
	//responsable de faire ces vérifications, 
	//puisque qu'il est beaucoup moins occupé

	ScanLine=y*Bitmap.ScanSize;
	int PxlPos = ScanLine+(int)(x/8);
	Bitmap.DIB[PxlPos]=Bitmap.DIB[PxlPos]|(128>>(x % 8));
}

bool CListData::Graph::GraphBitmap::DisplayBmp(int XPos, int YPos, HDC DestDC)
{
	int result=SetDIBitsToDevice(
		DestDC,			//Device context
		XPos, 			//Abcisse destination
		YPos,			//Ordonnée destination
		Bitmap.BmpInfo.bmiHeader.biWidth, //Largeur
		Bitmap.BmpInfo.bmiHeader.biHeight,//Hauteur
		0,				//Abcisse source
		0,				//Ordonnée source
		0,				//Début du premier scan line
		Bitmap.ScanNum, //Nombre de scan lines
		Bitmap.DIB,     //Adresse du bitmap
		&Bitmap.BmpInfo,
		DIB_RGB_COLORS  //La palette est en RGB
		);
	return true;
}

CListData::Graph::Graph(CListData *List)
{
	Zoom=false;
	bk = RGB(255,255,255);
	courbe = RGB(0,0,0);
	axes = RGB(0,0,0);
	DataList=List;
}

CListData::Graph::~Graph()
{
}

void CListData::Graph::ZoomBox(float StartWV, float StopWV, float AbsMin, float AbsMax)
{
	ZoomStartWV=StartWV;
	ZoomStopWV=StopWV;
	ZoomAbsMin=AbsMin;
	ZoomAbsMax=AbsMax;
	Zoom=true;
}

void CListData::Graph::FullView()
{
	Zoom=false;
}

void CListData::Graph::SetGraphMode(CDC* DestDC)
{
	DestDC->SetROP2(R2_COPYPEN);
	DestDC->SetMapMode(MM_TEXT);
}


void CListData::Graph::GiveMaxDlgInstance(CMaxDlg* MaxList)
{
	MaxInst=MaxList;
}


void CListData::Graph::DisplayGraph(CRect *Area, float XGraduation, int XGraduationLabel, float YGraduation, 
								   int YGraduationLabel, CDC *DestDC,BOOL Print)
{
	CBrush Brush;
	CPen pen;
	if (XGraduationLabel==0) XGraduationLabel=2;

	if (XGraduation==0) XGraduation=20;

	if (YGraduationLabel==0) YGraduationLabel=2;

	if (YGraduation==0) YGraduation=.5;


	SetGraphMode(DestDC);

	double XMin=DataList->GetXMin();
	double XMax=DataList->GetXMax();
	double YMin=DataList->GetYMin();
	double YMax=DataList->GetYMax();

	double Width=(XMax-XMin);
	double Height=(YMax-YMin);


	int Count=DataList->GetCount();
	double Interval=DataList->GetInterval();//DataList->GetInterval();


	Width=(XMax-XMin);

	Height=(YMax-YMin);
	double XScale=Width/(Area->right-Area->left);
	double YScale=Height/(Area->bottom-Area->top);

	if (YMin > -15*YScale)
	{
		YMin=-15*YScale;
		Height=(YMax-YMin);
		YScale=Height/(Area->bottom-Area->top);
	}


	Brush.CreateSolidBrush(bk);	//Une brush blanche est necessaire pour dessiner le rectangle
												//blanc du graphique
	pen.CreatePen(PS_SOLID, 1, courbe);
	DestDC->FillRect(Area, &Brush);

	DrawAxes(	Area, XMin, XMax, YMin, YMax, XGraduation,
				XGraduationLabel, YGraduation, YGraduationLabel, DestDC, Print);

	DestDC->SelectObject(pen);
	for (int i = 0; i<(Count-1); i++)
	{

		double YFrom=DataList->GetYData(i);
		double YTo=DataList->GetYData(i+1);
		double XFrom=DataList->GetXData(i);
		double XTo=DataList->GetXData(i+1);

		double x1=Area->left+(XFrom-XMin)/XScale;
		double y1=(Area->bottom)-(YFrom-YMin)/YScale;
		double x2=Area->left+(XTo-XMin)/XScale;
		double y2=(Area->bottom)-(YTo-YMin)/YScale;
		DrawLine(x1, y1, x2 ,y2, DestDC);
	}

	if ((Print)&&(MaxInst->m_List.GetItemCount()>0)) DrawMaxTable(Area, DestDC);
}


void CListData::Graph::DrawMaxTable(CRect* Area, CDC *DestDC)

{

	TEXTMETRIC TextSize;

	char LongueurOnde[6];
	CPen* OldPen;
	CPen newpen;
	OldPen=DestDC->GetCurrentPen();


	DestDC->GetTextMetrics(&TextSize);

	MaxInst->UpdateData(TRUE);

	/**/DrawLine(Area->right,Area->top,Area->right,Area->top+(MaxInst->m_List.GetItemCount()+1)*(TextSize.tmHeight+1), DestDC);

	/**/DrawLine(Area->right-TextSize.tmMaxCharWidth*8,Area->top,Area->right,Area->top, DestDC);

	DrawLine(Area->right-TextSize.tmMaxCharWidth*8,Area->top,Area->right-TextSize.tmMaxCharWidth*8,Area->top+(MaxInst->m_List.GetItemCount()+1)*(TextSize.tmHeight+1), DestDC);

	DrawLine(Area->right-TextSize.tmMaxCharWidth*8,Area->top,Area->right-TextSize.tmMaxCharWidth*23,Area->top, DestDC);

	DrawLine(Area->right-TextSize.tmMaxCharWidth*23,Area->top,Area->right-TextSize.tmMaxCharWidth*23,Area->top+(MaxInst->m_List.GetItemCount()+1)*(TextSize.tmHeight+1), DestDC);

	DrawLine(Area->right,Area->top+(MaxInst->m_List.GetItemCount()+1)*(TextSize.tmHeight+1),Area->right-TextSize.tmMaxCharWidth*23,Area->top+(MaxInst->m_List.GetItemCount()+1)*(TextSize.tmHeight+1), DestDC);

	DestDC->SetTextAlign(TA_LEFT|TA_TOP);

	DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*23+1,Area->top+1,"Longueur d'onde");

	DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*8+1,Area->top+1,"Abs");

	for(int i=0;i<MaxInst->m_List.GetItemCount();i++)

	{

		DrawLine(Area->right,Area->top+(i+1)*(TextSize.tmHeight+1),Area->right-TextSize.tmMaxCharWidth*23,Area->top+(i+1)*(TextSize.tmHeight+1), DestDC);

		MaxInst->m_List.GetItemText(i,0,LongueurOnde,6);

		DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*23+1,Area->top+(i+1)*(TextSize.tmHeight+1)+1,LongueurOnde);

	}

	for(int i=0;i<MaxInst->m_List.GetItemCount();i++)

	{

		MaxInst->m_List.GetItemText(i,1,LongueurOnde,6);

		DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*8+1,Area->top+(i+1)*(TextSize.tmHeight+1)+1,LongueurOnde);

	}

}


void CListData::Graph::SaveGraphBmp(CDC* Source, char *path)
{
	CBitmap* SourceBmp;
	BITMAP BmpInfo;

	SourceBmp=Source->GetCurrentBitmap();
	SourceBmp->GetBitmap(&BmpInfo);
}

void CListData::Graph::DrawAxes(CRect *Area, double XMin, double XMax, double YMin, double YMax,
								double XGraduation, int XGraduationLabel, double YGraduation, 
								int YGraduationLabel, CDC *DestDC, BOOL Print)
{
	CString LabelStr;
	char LabelChar[10];
	double XScale=(XMax-XMin)/(Area->right-Area->left);
	double YScale=(YMax-YMin)/(Area->bottom-Area->top);



	//YMin=-5/YScale;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, axes);
	DestDC->SelectObject(pen);
	DestDC->SetTextColor(axes);


	DestDC->SetBkMode(TRANSPARENT);
	int HorzGradSize=Area->Width()*.007692;

	int VertGradSize=Area->Height()*.01005;

	TEXTMETRIC TextSize;
	DestDC->SetTextAlign(TA_LEFT|TA_TOP);
	

	DestDC->GetTextMetrics(&TextSize);


	if (YMin>=0)
	{
		DrawLine(Area->left,Area->bottom,Area->right,Area->bottom,DestDC);
		DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*15,Area->bottom-0,"Longueur d'onde");
	}
	else
	{
		DrawLine(Area->left,Area->bottom+(YMin/YScale),Area->right,Area->bottom+(YMin/YScale),DestDC);
		DestDC->TextOut(Area->right-TextSize.tmMaxCharWidth*15,Area->bottom+(YMin/YScale)-0,"Longueur d'onde");
	}
	DestDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
	if (XGraduation)
		if ((YMin/YScale)<=-5)
			for(double i=Area->left;i<Area->right;i+=(XGraduation/XScale))
			{
				itoa(round((i-Area->left)*XScale+XMin),LabelChar,10);
				LabelStr=LabelChar;
				if (XGraduationLabel==0)
					if (i-Area->left==0)

					{
						DestDC->TextOut(i+15,Area->bottom+(YMin/YScale)+7,LabelStr);

						DrawLine(i,Area->bottom+(YMin/YScale)+VertGradSize/2,i,Area->bottom+(YMin/YScale)-VertGradSize/2,DestDC);

					}
					else

					{
						DestDC->TextOut(i,Area->bottom+(YMin/YScale)+7,LabelStr);

						DrawLine(i,Area->bottom+(YMin/YScale)+VertGradSize/2,i,Area->bottom+(YMin/YScale)-VertGradSize/2,DestDC);

					}
				else
					if ((int)round((i-(double)Area->left)/(XGraduation/XScale))%XGraduationLabel==0)
						if (i-Area->left==0)

						{

							DrawLine(i,Area->bottom+(YMin/YScale)+VertGradSize/2,i,Area->bottom+(YMin/YScale)-VertGradSize/2,DestDC);
							DestDC->TextOut(i+15,Area->bottom+(YMin/YScale)+7,LabelStr);

						}
						else

						{

							DrawLine(i,Area->bottom+(YMin/YScale)+VertGradSize/2,i,Area->bottom+(YMin/YScale)-VertGradSize/2,DestDC);
							DestDC->TextOut(i,Area->bottom+(YMin/YScale)+7,LabelStr);

						}

					else

					{

						DrawLine(i,Area->bottom+(YMin/YScale)+VertGradSize/4,i,Area->bottom+(YMin/YScale)-VertGradSize/4,DestDC);

					}

			}
		else
			for(double i=Area->left;i<Area->right;i+=(XGraduation/XScale))
			{
				itoa(round((i-Area->left)*XScale+XMin),LabelChar,10);
				LabelStr=LabelChar;
				if (XGraduationLabel==0)
					if (i-Area->left==0)

					{
						DestDC->TextOut(i+15,Area->bottom-5,LabelStr);

						DrawLine(i,Area->bottom,i,Area->bottom-VertGradSize,DestDC);

					}
					else

					{
						DestDC->TextOut(i,Area->bottom-5,LabelStr);

						DrawLine(i,Area->bottom,i,Area->bottom-VertGradSize,DestDC);

					}
				else
					if ((int)round((i-(double)Area->left)/(XGraduation/XScale))%XGraduationLabel==0)
						if (i-Area->left==0)

						{
							DestDC->TextOut(i+15,Area->bottom-5,LabelStr);

							DrawLine(i,Area->bottom,i,Area->bottom-VertGradSize,DestDC);

						}
						else

						{
							DestDC->TextOut(i,Area->bottom-5,LabelStr);

							DrawLine(i,Area->bottom,i,Area->bottom-VertGradSize,DestDC);

						}

					else

						DrawLine(i,Area->bottom,i,Area->bottom-VertGradSize/2,DestDC);
			}
	DestDC->SetTextAlign(TA_LEFT|TA_TOP);
	if (XMin>=0)
	{
		DrawLine(Area->left,Area->bottom,Area->left,Area->top,DestDC);
		DestDC->TextOut(Area->left+40,Area->top+5,"Abs");
	}
	else
	{
		DrawLine(Area->left-(XMin/XScale),Area->bottom,Area->left-(XMin/XScale),Area->top,DestDC);
		DestDC->TextOut(Area->left-(XMin/XScale)+40,Area->top+5,"Abs");
	}
	DestDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
	if (YGraduation)
		if ((XMin/XScale)<=-5)
			for(double i=Area->bottom;i>Area->top;i-=(YGraduation/YScale))
			{
				if (YGraduationLabel==0)
					if ((i-Area->bottom)<0)
					{
						itoa((i-Area->bottom)*-YScale+YMin,LabelChar,10);
						LabelStr=LabelChar;
						LabelStr+=".";
						itoa((((i-Area->bottom)*-YScale+YMin)-(int)((i-Area->bottom)*-YScale+YMin))*10,LabelChar,10);
						LabelStr+=LabelChar;
						DestDC->TextOut(Area->left-(XMin/XScale)-7,i,LabelStr);

						DrawLine(Area->left-(XMin/XScale)+HorzGradSize/2,i,Area->left-(XMin/XScale)-HorzGradSize/2,i,DestDC);
					}
					else DrawLine(Area->left-(XMin/XScale)+HorzGradSize/4,i,Area->left-(XMin/XScale)-HorzGradSize/4,i,DestDC);
				else
				if ((int)round((i-(double)Area->bottom)/(YGraduation/YScale))%YGraduationLabel==0)
					if ((i-Area->bottom)<0)
					{
						itoa((i-Area->bottom)*-YScale+YMin,LabelChar,10);
						LabelStr=LabelChar;
						LabelStr+=".";
						itoa((((i-Area->bottom)*-YScale+YMin)-(int)((i-Area->bottom)*-YScale+YMin))*10,LabelChar,10);
						LabelStr+=LabelChar;
						DestDC->TextOut(Area->left-(XMin/XScale)-7,i,LabelStr);

						DrawLine(Area->left-(XMin/XScale)+HorzGradSize/2,i,Area->left-(XMin/XScale)-HorzGradSize/2,i,DestDC);
					}

					else DrawLine(Area->left-(XMin/XScale)+HorzGradSize/4,i,Area->left-(XMin/XScale)-HorzGradSize/4,i,DestDC);

				else DrawLine(Area->left-(XMin/XScale)+HorzGradSize/4,i,Area->left-(XMin/XScale)-HorzGradSize/4,i,DestDC);
			}
		else
			for(double i=Area->bottom;i>Area->top;i-=(YGraduation/YScale))
			{
				DrawLine(Area->left,i,Area->left+HorzGradSize,i,DestDC);
				if (YGraduationLabel==0)
					if ((i-Area->bottom)<0)
					{
						itoa((i-Area->bottom)*-YScale+YMin,LabelChar,10);
						LabelStr=LabelChar;
						LabelStr+=".";
						itoa((((i-Area->bottom)*-YScale+YMin)-(int)((i-Area->bottom)*-YScale+YMin))*10,LabelChar,10);
						LabelStr+=LabelChar;
						DestDC->TextOut(Area->left+15,i,LabelStr);

						DrawLine(Area->left,i,Area->left+HorzGradSize,i,DestDC);
					}
					else

					{

						DrawLine(Area->left,i,Area->left+HorzGradSize/2,i,DestDC);

					}
				else
				if ((int)round(((i-(double)Area->bottom)/(YGraduation/YScale)))%YGraduationLabel==0)
					if ((i-Area->bottom)<0)
					{

						DrawLine(Area->left,i,Area->left+HorzGradSize,i,DestDC);
						itoa((i-Area->bottom)*-YScale+YMin,LabelChar,10);
						LabelStr=LabelChar;
						LabelStr+=".";
						itoa((((i-Area->bottom)*-YScale+YMin)-(int)((i-Area->bottom)*-YScale+YMin))*10,LabelChar,10);
						LabelStr+=LabelChar;
						DestDC->TextOut(Area->left+15,i,LabelStr);
					}

					else DrawLine(Area->left,i,Area->left+HorzGradSize/2,i,DestDC);

				else DrawLine(Area->left,i,Area->left+HorzGradSize/2,i,DestDC);
			}

	//On met les graduations que si XGraduation
	//n'est pas nulle, si c nulle, c'est que
	//l'on veut pas graduer l'axe des abcisses.
}

void CListData::Graph::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, CDC* DestDC)
{
	DestDC->MoveTo(x1,y1);
	DestDC->LineTo(x2,y2);
}

XY CListData::FindMax(double beginx, double endx)
{
	int i;
	XY xymax;
	xymax.x = 0;
	xymax.y = 0;

	bool begin = true;

	double next=0;

	if(beginx > endx)
	{
		int buf = beginx;
		beginx = endx;
		endx = buf;
	}

	int pos = (int)((beginx - GetXMin()) / Interval);
	if(pos<0)
		pos = 0;

	int end = (int)((endx - GetXMin()) / Interval)+1;
	if(end > count)
		end = count;

	for(i=pos; i<end; i++)
	{
		if(begin)
		{
			xymax.y = GetYData(i);
			xymax.x = GetXData(i);
			begin = false;
		}
		else
		{
			next = GetYData(i);
			if(next > xymax.y)
			{
				xymax.x = GetXData(i);
				xymax.y = next;
			}
		}
	}
	
	return xymax;
}

XY CListData::GetExtrapolated(double value)
{
	double BeforeCurrent;

//Calulons l'indice précédant value ou celui qui lui
//corespond si nous sommes pile dessus.  Avant, nous
//devont
//Enlever XMin, car nous avons besoins x relativement
//au debut de la suite de donnees.

	BeforeCurrent=value-XMin;
	BeforeCurrent=BeforeCurrent/Interval;

//Si nous sommes pile dessus, BeforeCurrent devrait
//etre entier.  Sinon, il a une partie float que
//nous devons tronquer pour avoir l'indice
//de la donnée précédante.
	XY xymax;

	if (BeforeCurrent-(int)BeforeCurrent==0)
	{
		//Nous sommes pile sur le point!
		xymax.x = GetXData((int)BeforeCurrent);
		xymax.y = GetYData((int)BeforeCurrent);
		//Hé que la vie est belle!
		return xymax;
	}
	else
	{
		double pente;
		//Nous allons extrapoler le point sur
		//la droite reliant les 2 données entourant
		//le curseur.  Nous aurons besoin de la pente.
		//l'origine sera centrée sur la donnée
		//précédente, alors le parametre b est nul.

		XY xymax2;
		//Les choses vont etre un peu plus compliquées!
		xymax.x = GetXData((int)BeforeCurrent);
		xymax.y = GetYData((int)BeforeCurrent);
		xymax2.x = GetXData((int)BeforeCurrent+1);
		xymax2.y = GetYData((int)BeforeCurrent+1);
		//Nous connaissons maintenant le point
		//précédant et le point suivant
		//Calculont la pente de la droite reliant
		//les 2 points
		pente = (xymax2.y-xymax.y)/(xymax2.x-xymax.x);
		//Conditions actuelles sur lesquelles nous
		//nous basons pour extrapoler:
		// -le point (value,y) est sur la droite
		//  reliant xymax et xymax2.
		// -pente est la pente de la droite reliant
		//  xymax xymax2.
		// -Notre droite imaginaire est dans un plan
		//  dont l'origine est située sur xymax
		// -b est nul, puisque la droite passe par
		//  l'origine.  Donc il est inutile.
		// -Apres que la pente soit calculée, xymax2
		//  est inutile, nous allons le recycler pour
		//  sauver de la memoire.  Il deviendra le
		//  point extrapolé.
		
		//Puisque le plan imaginaire est sur xymax,
		//nous devons soustraire xymax.x de value
		//pour le rendre relatif au plan imaginaire
		//xymax2 sera alors relatif au plan imaginaire
		xymax2.x=value-xymax.x;
		//Pour trouver le point (xymax2.x,xymax2.y),
		//nous devons faire xymax2.y=pente*xymax2.x+b.
		// et b est nul, donc on l'élimine.
		xymax2.y=pente*xymax2.x;
		//Maintenant que nos avons le point extrapolé
		//dans notre plan imaginaire, nous devons le
		//rendre relatif au plan
		//contenant l'étendu de donnée.
		xymax2.x+=xymax.x;
		xymax2.y+=xymax.y;

		//xymax2 est le point ou se trouve le curseur.
		return xymax2;
	}
}
