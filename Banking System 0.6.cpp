// 17.12.27
// Banking System ver.0.6

// [�����Ȳ]
// 17.12.04 Banking System Ver 0.1 ����
// 17.12.08 - ���� ������ �߰�
// 17.12.14 - const ���� �߰�
// 17.12.20 - Ŭ���� ���� ���� ����, ��Ʈ�� Ŭ���� ����
// 17.12.27 - 

#include <iostream>
#include <cstring>

using namespace std;

enum { MAKE = 1, DEPOSIT, WITHDRAW, SHOWINFO, EXIT };

enum { GRADE_A = 7, GRADE_B = 4, GRADE_C = 2 };

class Account {
private:
	int accID;		// ���¹�ȣ
	int balance;	// �ܾ�
	char* cusName;	// ���̸�
public:
	Account(int accID, int money, char* Name);
	Account(const Account& copy);
	int getID(void) const;
	virtual void Deposit(int money) = 0;
	int WithDraw(int money);
	void showAccInfo(void) const;
	~Account();
};

Account::Account(int accID, int money, char* Name) : accID(accID), balance(money) {
	cusName = new char[strlen(Name) + 1];
	strcpy_s(cusName, strlen(Name) + 1, Name);
}

Account::Account(const Account& copy) : accID(copy.accID), balance(copy.balance) {
	cusName = new char[strlen(copy.cusName) + 1];
	strcpy_s(cusName, strlen(copy.cusName) + 1, copy.cusName);
}

void Account::Deposit(int money)
{
	balance += money;
}

int Account::getID(void) const
{
	return accID;
}

int Account::WithDraw(int money) {
	if (balance < money)
		return 0;
	balance -= money;
	return money;
}

void Account::showAccInfo(void) const
{
	cout << "=====================" << endl;
	cout << "����ID: " << accID << endl;
	cout << "���̸�: " << cusName << endl;
	cout << "�ܾ�: " << balance << endl;
	cout << "=====================" << endl;
}

Account::~Account() {
	delete[] cusName;
}


/*
* Ŭ���� �̸� : NormalAccount
* Ŭ���� ���� : ���뿹�ݰ���(Account ���)
*/
class NormalAccount : public Account {
private:
	int NAratio;
public:
	NormalAccount(int ID, int money, char* name, int ratio)
		: Account(ID, money, name), NAratio(ratio) {}

	int getNAratio(int money)
	{
		return (int)money*NAratio / 100;
	}

	void Deposit(int money)
	{
		Account::Deposit(money);
		Account::Deposit((int)money*NAratio / 100);
	}
};


/*
* Ŭ���� �̸� : HighCreditAccount
* Ŭ���� ���� : �ſ�ŷڰ���(Account ���)
*/
class HighCreditAccount :public NormalAccount {
private:
	int HCAGrade;
public:
	HighCreditAccount(int ID, int money, char* name, int ratio, int grade)
		: NormalAccount(ID, money, name, ratio), HCAGrade(grade) {}

	int HCAPlusInter(int money)
	{
		switch (HCAGrade)
		{
		case 1:
			return (int)money*GRADE_A / 100;
		case 2:
			return (int)money*GRADE_B / 100;
		case 3:
			return (int)money*GRADE_C / 100;
		default:
			break;
		}
	}

	void Deposit(int money)
	{
		Account::Deposit(money + NormalAccount::getNAratio(money) + HighCreditAccount::HCAPlusInter(money));
	}
};


/*
* Ŭ���� �̸� : AccountHandler
* Ŭ���� ���� : control class
*/
class AccountHandler {
private:
	Account * Acc[100];
	int accCount = 0;	// ���� ����
public:
	AccountHandler();
	void showMenu(void) const;
	void createAcc(void);
	void deposit(void);
	void withDraw(void);
	void showAllInfo(void) const;
	int getAccCount(void);
	~AccountHandler();
};

AccountHandler::~AccountHandler() {
	for (int i = 0; i < accCount; ++i)
		delete Acc[i];
}

AccountHandler::AccountHandler() : accCount(0) {}

int AccountHandler::getAccCount(void)
{
	return accCount;
}

void AccountHandler::showMenu(void) const
{
	cout << "======================" << endl;
	cout << "Banking System ver 0.6" << endl;
	cout << "======================" << endl;
	cout << "         �޴�" << endl;
	cout << "     1. ���� ����" << endl;
	cout << "     2. �Ա�" << endl;
	cout << "     3. ���" << endl;
	cout << "     4. ���� ��ȸ" << endl;
	cout << "     5. ����" << endl;
	cout << " #����====> ";
}
void AccountHandler::createAcc(void)
{
	int ch;

	int id;
	int balance;
	char name[20];
	int ratio;

	int creditGrade;

	cout << "[������������]" << endl;
	cout << "1. ���뿹�ݰ��� 2. �ſ�ŷڰ��� " << endl;
	cout << "����: ";
	cin >> ch;

	switch (ch)
	{
	case 1:
		cout << "[���뿹�ݰ��� ����]" << endl;
		cout << "�̸�: "; cin >> name;
		cout << "���¹�ȣ: "; cin >> id;
		cout << "�Աݾ�: "; cin >> balance;
		cout << "������: "; cin >> ratio;
		Acc[accCount++] = new NormalAccount(id, balance, name, ratio);
		cout << "---���°����Ϸ�!" << endl;
		cout << endl;
		break;
	case 2:
		cout << "[�ſ�ŷڰ��� ����]" << endl;
		cout << "�̸�: "; cin >> name;
		cout << "���¹�ȣ: "; cin >> id;
		cout << "�Աݾ�: "; cin >> balance;
		cout << "������: "; cin >> ratio;
		cout << "�ſ���(Ato1,Bto2,Cto3): "; cin >> creditGrade;
		Acc[accCount++] = new HighCreditAccount(id, balance, name, ratio, creditGrade);
		cout << "---���°����Ϸ�!" << endl;
		cout << endl;
		break;

	default:
		break;
	}

}
void AccountHandler::deposit()
{
	int id;
	int money;
	cout << "[�� ��]" << endl;
	cout << "���¹�ȣ�� �Է��ϼ���: "; cin >> id;
	cout << "�Ա��� �ݾ��� �Է��ϼ���: "; cin >> money;

	for (int i = 0; i < accCount; ++i) {
		if (Acc[i]->getID() == id) {
			Acc[i]->Deposit(money);
			cout << "�Ա� �Ϸ�!" << endl;
			return;
		}
	}
	cout << "��ȿ���� ���� ID�Դϴ�." << endl;

}
void AccountHandler::withDraw()
{
	int id;
	int money;
	cout << "[�� ��]" << endl;
	cout << "���¹�ȣ�� �Է��ϼ���: "; cin >> id;
	cout << "����� �ݾ��� �Է��ϼ���: "; cin >> money;

	for (int i = 0; i < accCount; ++i) {
		if (Acc[i]->getID() == id) {
			if (Acc[i]->WithDraw(money) == 0) {
				cout << "�ܾ��� �����մϴ�" << endl;
				return;
			}
			else {
				cout << "��� �Ϸ�!" << endl;
				return;
			}
		}
	}
	cout << "��ȿ���� ���� ID �Դϴ�." << endl;
}

void AccountHandler::showAllInfo(void) const
{
	for (int i = 0; i < accCount; ++i) {
		cout << "[" << i + 1 << "]" << endl;
		Acc[i]->showAccInfo();
	}
}



/* --------------------
* main()
* ------------------- */
int main()
{
	AccountHandler manager;
	int choice;

	while (1) {
		manager.showMenu();
		cin >> choice;

		switch (choice)
		{
		case MAKE:
			manager.createAcc();
			break;
		case DEPOSIT:
			manager.deposit();
			break;
		case WITHDRAW:
			manager.withDraw();
			break;
		case SHOWINFO:
			manager.showAllInfo();
			break;
		case EXIT:
			return 0;
		default:
			break;
		}
		system("pause");
		system("cls");

	}
	return 0;
}