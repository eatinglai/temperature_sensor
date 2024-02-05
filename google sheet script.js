//google sheet appscript
function doGet(e) { 
  Logger.log( JSON.stringify(e) );
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'Unrecognizied Parameters';
  }
  else {
    var sheet_id = 'ABCD'; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1; 
    var rowData = [];

    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Taipei", 'HH:mm:ss');
    rowData[1] = Curr_Time; // Time in column B

    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
      case 'temperature':
      rowData[2] = value; // Temperature in column C
      result += 'Temperature Written on column C'; 
      break;

      case 'env_temperature':
      rowData[3] = value; // Temperature in column D
      result += ' ,env_Temperature Written on column D'; 
      break;

      case 'env_humidity':
      rowData[4] = value; // Humidity in column E
      result += ' ,env_Humidity Written on column E'; 
      break; 

      default:
      result = "unsupported parameter";
      }
    }
    
    Logger.log(JSON.stringify(rowData));
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  return ContentService.createTextOutput(result);
}
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}