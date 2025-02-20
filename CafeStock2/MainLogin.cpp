#include "MainLogin.h"
#include "Menumain.h"
#include "Register.h"
#include "DatabaseHandler.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data;

[STAThread]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew CafeStock::MainLogin());
    return 0;
}

System::Void CafeStock::MainLogin::bttnLogin_Click(System::Object^ sender, System::EventArgs^ e) {
    // Get input from textboxes
    System::String^ username = this->txtUsername->Text;
    System::String^ password = this->txtPassword->Text;

    // Input validation
    if (System::String::IsNullOrWhiteSpace(username)) {
        MessageBox::Show("Username cannot be empty.", "Validation Error",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    if (System::String::IsNullOrWhiteSpace(password)) {
        MessageBox::Show("Password cannot be empty.", "Validation Error",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    // Create database handler with Supabase URL and API key
    String^ supabaseUrl = "https://cefsumddrfcatzaswddb.supabase.co/rest/v1";
    String^ apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNlZnN1bWRkcmZjYXR6YXN3ZGRiIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Mzk5MzQzNDQsImV4cCI6MjA1NTUxMDM0NH0.x0cSDoOt7VWnqwubHYLc9AeeOD3miWYalLB64Va25ls";
    DatabaseHandler^ db = gcnew DatabaseHandler(supabaseUrl, apiKey);

    // Construct query URL using Supabase filters
    String^ tableName = "users";
    String^ url = tableName + "?username=eq." + username + "&password=eq." + password;

    // Execute GET request
    DataTable^ result = db->getData(url);

    if (result == nullptr) {
        MessageBox::Show("Database query execution failed.", "Database Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    // Check Query Result
    if (result != nullptr && result->Rows->Count > 0) {
        MessageBox::Show("Login successful!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);

        // Hide current form and show main menu
        this->Hide();
        Menumain^ newForm = gcnew Menumain();
        newForm->ShowDialog();
        this->Close();
    }
    else {
        MessageBox::Show("Invalid username or password.", "Login Failed",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}


System::Void CafeStock::MainLogin::label7_Click(System::Object^ sender, System::EventArgs^ e) {
    this->Hide();
    Register^ regForm = gcnew Register();
    regForm->ShowDialog();
    this->Close();
}
