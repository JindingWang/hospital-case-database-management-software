# coding=utf-8
import xlrd
import datetime

if __name__ == "__main__":
    sql_file = open("sql_file.txt", "w")
    # 打开文件
    data = xlrd.open_workbook("C:\\Users\\Anumidum\\Desktop\\record.xlsx")
    # 查看工作表
    data.sheet_names()
    print("sheets：" + str(data.sheet_names()))
    # 打印data.sheet_names()可发现，返回的值为一个列表，通过对列表索引操作获得工作表1
    table = data.sheet_by_index(0)

    # 获取行数和列数
    rows = table.nrows
    cols = table.ncols
    print("总行数: ", rows)
    print("总列数: ", cols)
    record = ("id", "pathologyNumber", "type", "sex", "age", "institution", "sampleDate", "dyeingDate",
              "scanDate", "sectionPath", "position", "visibleDiagnosis", "clinicalDiagnosis",
              "cryoDiagnosis", "PathologicDiagnosis", "aiDiagnosis")
    index = table.row_values(1)
    index = list(map(lambda x: str(x).strip(), index))
    index_dict = {}
    for i in range(len(index)):
        li = table.col_values(i)[2:]
        li = list(map(lambda x: str(x).strip().replace('"', '\\"').replace("'", "\\'"), li))
        index_dict[index[i]] = li.copy()

    begin_id = 1
    for i in range(rows - 2):
        string = "INSERT INTO SectionTable VALUES ({}".format(begin_id+i)
        for j in range(1, len(record)):
            if record[j] in index:
                if record[j] == "age":
                    string += ",{}".format(int(float(index_dict[record[j]][i])))
                elif record[j] in ["sampleDate", "dyeingDate", "scanDate"]:
                    begin_date = datetime.datetime(1900, 1, 1, 0, 0, 0)
                    date = begin_date + datetime.timedelta(days=int(float(index_dict[record[j]][i]))-2)
                    year = str(date.year)
                    month = str(date.month)
                    if len(month) == 1:
                        month = "0" + month
                    day = str(date.day)
                    if len(day) == 1:
                        day = "0" + day
                    string += ",'{}-{}-{}'".format(year, month, day)
                else:
                    string += ",'{}'".format(index_dict[record[j]][i])
            else:
                string += ",''"
        string += ");\n"
        #print(string)
        sql_file.write(string)

    sql_file.close()