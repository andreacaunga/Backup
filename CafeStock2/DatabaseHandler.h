#pragma once
using namespace System;
using namespace System::Data;
using namespace System::Net::Http;
using namespace System::Text;
using namespace System::Windows::Forms;

public ref class DatabaseHandler {
private:
    String^ baseUrl;
    String^ apiKey;
    HttpClient^ client;

public:
    DatabaseHandler(String^ url, String^ key);
    ~DatabaseHandler();
    DataTable^ getData(String^ tableName);
    bool insertData(String^ tableName, String^ jsonData);
    bool updateData(String^ tableName, String^ jsonData, String^ id);
    bool deleteData(String^ tableName, String^ id);
};
