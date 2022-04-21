#ifndef DropBoxData_h__
#define DropBoxData_h__

BEGIN(Client)
class CItemData;
class CDropBoxData
{
public:
	CDropBoxData(void);

public:
	vector<CItemData> GetItemData(_int i) { return m_vecIteamData[i]; }

private:
	std::vector<CItemData> m_vecIteamData[10];
};

END
#endif // DropBoxData_h__
