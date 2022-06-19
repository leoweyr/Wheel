#pragma once

#include "windows.h"
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include <string>

namespace WindowsExcel{
    class Excel{
        public:
            CApplication ExcelApp;
            CWorkbook ExcelWorkbook;
            CWorkbooks ExcelWorkbooks;
            CWorksheet ExcelWorksheet;
            CWorksheets ExcelWorksheets;
            CRange ExcelRange;
            bool isExcelServerRuntime = false;

            bool Workbook_open(std::string workbookPath);
            bool Save(std::string workbookPath);
    };

    bool Excel::Workbook_open(std::string workbookPath) {
    }
    bool Excel::Save(std::string workbookPath=""){
        ExcelWorkbooks.ReleaseDispatch();
    }
}

