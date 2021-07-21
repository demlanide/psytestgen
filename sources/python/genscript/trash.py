import sqlite3
from openpyxl import load_workbook
import numpy as np
import math
from sklearn.linear_model import LinearRegression

limit = 4  # минимальный размер выборки для высчитывания коэффициента корреляции

connection = sqlite3.connect("/Users/dameli/Desktop/diploma/generator/sources/gendb.db")
crsr = connection.cursor()

sql_command = "SELECT id_trait FROM traits"
crsr.execute(sql_command)
ans = crsr.fetchall()
lst = ans

for i in lst:
    sql_command = "SELECT count(*) FROM results WHERE id_trait = " + str(i[0])
    crsr.execute(sql_command)
    ans = crsr.fetchall()
    ans = str(ans[0][0])
    sql_command = "UPDATE traits SET quantity = " + ans + " WHERE id_trait = " + str(i[0])
    crsr.execute(sql_command)
connection.commit()