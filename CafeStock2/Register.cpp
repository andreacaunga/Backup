#include "Register.h"
#include "MainLogin.h"
#include "DatabaseHandler.h"

using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms;

// Switch to Login Form
System::Void CafeStock::Register::lblRegis_Click(System::Object^ sender, System::EventArgs^ e) {
    this->Hide();
    MainLogin^ loginForm = gcnew MainLogin();
    loginForm->ShowDialog();
    this->Close();
}

// Registration Button Click Event
System::Void CafeStock::Register::button1_Click(System::Object^ sender, System::EventArgs^ e) {
    // Get user input from form fields
    System::String^ username = txtUsername->Text;
    System::String^ password = txtPassword->Text;
    System::String^ confirmPassword = txtConfirmPass->Text;

    // Validate inputs
    if (System::String::IsNullOrWhiteSpace(username) ||
        System::String::IsNullOrWhiteSpace(password) ||
        System::String::IsNullOrWhiteSpace(confirmPassword))
    {
        MessageBox::Show("All fields are required!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    // Check if passwords match
    if (password != confirmPassword) {
        MessageBox::Show("Passwords do not match!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    // Hash password (Replace with SHA-256 or bcrypt in production)
    System::String^ hashedPassword = password; // Placeholder: Replace with actual hashing

    // Create Database Handler (With Supabase URL and API Key)
    String^ supabaseUrl = "https://cefsumddrfcatzaswddb.supabase.co/rest/v1";
    String^ apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNlZnN1bWRkcmZjYXR6YXN3ZGRiIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Mzk5MzQzNDQsImV4cCI6MjA1NTUxMDM0NH0.x0cSDoOt7VWnqwubHYLc9AeeOD3miWYalLB64Va25ls";
    DatabaseHandler^ db = gcnew DatabaseHandler(supabaseUrl, apiKey);
    String^ jsonData = "{\"username\":\"" + username + "\",\"password\":\"" + hashedPassword + "\"}";
    MessageBox::Show("Insert URL: " + supabaseUrl + "/public.users", "Debug Info");
    MessageBox::Show("JSON Data: " + jsonData, "Debug Info");


    try {
        // ✅ Step 1: Check if Username Already Exists Using GET
        String^ tableName = "users";
        String^ url = tableName + "?username=eq." + username;

        DataTable^ result = db->getData(url);

        if (result != nullptr && result->Rows->Count > 0) {
            MessageBox::Show("Username already exists. Choose another.", "Registration Failed", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // ✅ Step 2: Insert New User Using POST
        String^ jsonData = "{\"username\":\"" + username + "\",\"password\":\"" + hashedPassword + "\"}";

        bool insertSuccess = db->insertData(tableName, jsonData);

        if (insertSuccess) {
            // Show success message
            MessageBox::Show("Registration successful!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);

            // Redirect to Login Form
            this->Hide();
            MainLogin^ loginForm = gcnew MainLogin();
            loginForm->ShowDialog();
            this->Close();
        }
        else {
            MessageBox::Show("Registration failed. Please try again.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
    catch (Exception^ ex) {
        MessageBox::Show(ex->Message, "Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}
