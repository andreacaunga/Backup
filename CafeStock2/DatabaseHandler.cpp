#include "DatabaseHandler.h"

DatabaseHandler::DatabaseHandler(String^ url, String^ key) {
    baseUrl = url;
    apiKey = key;
    client = gcnew HttpClient();
    client->DefaultRequestHeaders->Add("apikey", apiKey);
    client->DefaultRequestHeaders->Add("Authorization", "Bearer " + apiKey);
}

DatabaseHandler::~DatabaseHandler() {
    delete client;
}

// ---------------------- GET Data ----------------------
DataTable^ DatabaseHandler::getData(String^ tableName) {
    try {
        Uri^ uri = gcnew Uri(baseUrl + "/" + tableName);
        HttpResponseMessage^ response = client->GetAsync(uri)->Result;

        if (!response->IsSuccessStatusCode) {
            MessageBox::Show("Failed to fetch data: " + response->ReasonPhrase, "Database Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
            return nullptr;
        }

        String^ jsonResponse = response->Content->ReadAsStringAsync()->Result;
        DataTable^ dt = gcnew DataTable();
        dt->Columns->Add("Response");
        DataRow^ row = dt->NewRow();
        row["Response"] = jsonResponse;
        dt->Rows->Add(row);

        return dt;
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error: " + ex->Message, "Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return nullptr;
    }
}

// ---------------------- INSERT Data ----------------------
bool DatabaseHandler::insertData(String^ tableName, String^ jsonData) {
    try {
        Uri^ uri = gcnew Uri(baseUrl + "/" + tableName);
        HttpContent^ content = gcnew StringContent(jsonData, Encoding::UTF8, "application/json");
        HttpResponseMessage^ response = client->PostAsync(uri, content)->Result;

        return response->IsSuccessStatusCode;
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error: " + ex->Message, "Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
    }
}

// ---------------------- UPDATE Data ----------------------
bool DatabaseHandler::updateData(String^ tableName, String^ jsonData, String^ id) {
    try {
        Uri^ uri = gcnew Uri(baseUrl + "/" + tableName + "?id=eq." + id);
        HttpContent^ content = gcnew StringContent(jsonData, Encoding::UTF8, "application/json");
        HttpRequestMessage^ request = gcnew HttpRequestMessage(gcnew HttpMethod("PATCH"), uri);

        request->Content = content;
        HttpResponseMessage^ response = client->SendAsync(request)->Result;

        return response->IsSuccessStatusCode;
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error: " + ex->Message, "Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
    }
}

// ---------------------- DELETE Data ----------------------
bool DatabaseHandler::deleteData(String^ tableName, String^ id) {
    try {
        Uri^ uri = gcnew Uri(baseUrl + "/" + tableName + "?id=eq." + id);
        HttpResponseMessage^ response = client->DeleteAsync(uri)->Result;

        return response->IsSuccessStatusCode;
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error: " + ex->Message, "Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
    }
}
