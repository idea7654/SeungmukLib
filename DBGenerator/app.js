const csv = require('csv-parser');
const fs = require('fs');

const csvPath = './QueryList.csv';

//const QueryInputPath = '../DB/ContentsQueryInput.hhp';
//const QueryReturnPath = '../DB/ContentsQueryReturn.hpp';
const QueryInputPath = './ContentsQueryInput.hpp';
const QueryReturnPath = './ContentsQueryReturn.hpp';

let removeReg = /[`~!@#$%^&*()_|+\-=?;:'",.<>\{\}\[\]\\\/ ]/g;

let QueryReturn = `#pragma once
#include "pch.h"
#include <sstream>

template <typename T>
T GetAs(const std::string& s) {
    std::stringstream ss{s};
    T t;
    ss >> t;
    return t;
}
`;

let InputQuery = `#pragma once
#include "pch.h"
`;

rowArr = [];
fs.createReadStream(csvPath)
    .pipe(csv())
    .on('data', (row) => {
        rowArr.push(row);
    })
    .on('end', () => {
        for (let i in rowArr) {
            let QueryName = rowArr[i].QUERY_NAME;
            let DBType = rowArr[i].DB_TYPE;
            let QueryFrom = rowArr[i].QUERY_FROM;
            let InputList = rowArr[i].INPUT_LIST;
            let OutputList = rowArr[i].OUTPUT_LIST;
            let OutputListType = rowArr[i].OUTPUT_LIST_TYPE;

            //ReturnQuery
            if (OutputList != '') {
                QueryReturn += `
class ${QueryName}_Return : public QueryReturn
{
public:
    ${(function () {
        typeArr = [];
        varArr = [];

        OutputList.split(' ').map((element) => {
            element = element.replace(',', '');
            varArr.push(element);
        });

        OutputListType.split(' ').map((element) => {
            element = element.replace(',', '');
            typeArr.push(element);
        });

        let text;
        for (let i in typeArr) {
            //i == 0 ? (text += `std::vector<${typeArr[i]}> ${varArr[i]};`) : (text += `\n    std::vector<${typeArr[i]}> ${varArr[i]};`);
            let dummyStr = typeArr[i];
            let dummy = Array.from(dummyStr.matchAll('\\[(.*?)\\]'), (match) => `${match[0]}`);
            if (dummy.length >= 1) {
                dummyStr = dummyStr.replace(removeReg, '');
                i == 0 ? (text += `std::vector<${dummyStr}> ${varArr[i]};`) : (text += `\n    std::vector<${dummyStr}> ${varArr[i]};`);
            } else {
                i == 0 ? (text += `${dummyStr} ${varArr[i]};`) : (text += `\n    ${dummyStr} ${varArr[i]};`);
            }
        }
        text = text.replace('undefined', '');
        return text;
    })()}

    ${(function () {
        if (DBType == 'MYSQL') {
            return `virtual void ParseQueryResult(MYSQL_RES* sql_result) override
    {
        MYSQL_ROW row;
        
        while((row = mysql_fetch_row(sql_result)))
        {
            ${(function () {
                typeArr = [];
                varArr = [];

                OutputList.split(' ').map((element) => {
                    element = element.replace(',', '');
                    varArr.push(element);
                });

                OutputListType.split(' ').map((element) => {
                    element = element.replace(',', '');
                    typeArr.push(element);
                });

                let text;
                for (let i in typeArr) {
                    let dummyStr = typeArr[i];
                    let dummy = Array.from(dummyStr.matchAll('\\[(.*?)\\]'), (match) => `${match[0]}`);
                    if (dummy.length >= 1) {
                        dummyStr = dummyStr.replace(removeReg, '');
                        i == 0
                            ? (text += `${varArr[i]}.push_back(GetAs<${dummyStr}>(row[${i}]));`)
                            : (text += `\n    ${varArr[i]}.push_back(GetAs<${dummyStr}>(row[${i}]));`);
                    } else {
                        i == 0
                            ? (text += `${varArr[i]} = GetAs<${dummyStr}>(row[${i}]);`)
                            : (text += `\n    ${varArr[i]} = GetAs<${dummyStr}>(row[${i}]);`);
                    }
                }
                text = text.replace('undefined', '');

                return text;
            })()}
        }
    }`;
        } else {
            return `virtual void ParseQueryResult(redisReply* sql_result) override
    {
        ${(function () {
            typeArr = [];
            varArr = [];

            OutputList.split(' ').map((element) => {
                element = element.replace(',', '');
                varArr.push(element);
            });

            OutputListType.split(' ').map((element) => {
                element = element.replace(',', '');
                typeArr.push(element);
            });

            let text;

            for (let i in typeArr) {
                let dummyStr = typeArr[i];
                let dummy = Array.from(dummyStr.matchAll('\\[(.*?)\\]'), (match) => `${match[0]}`);
                if (dummy.length >= 1) {
                    text += `for(int i = 0; i < sql_result->elements; i++)\n        {\n            `;
                    i == 0
                        ? (text += `${varArr[i]}.push_back(GetAs<${typeArr[i]}>(sql_result->element[i]->str));`)
                        : (text += `\n    ${varArr[i]}.push_back(GetAs<${typeArr[i]}>(sql_result->element[i]->str));`);
                    text += '\n        }';
                } else {
                    i == 0
                        ? (text += `${varArr[i]} = GetAs<${typeArr[i]}>(sql_result->str);`)
                        : (text += `\n    ${varArr[i]} = GetAs<${typeArr[i]}>(sql_result->str);`);
                }
            }

            text = text.replace('undefined', '');

            return text;
        })()}
    }`;
        }
    })()}    
};
`;
            }
            //ReturnQuery End

            //InputQuery
            InputQuery += `
class ${QueryName} : public QueryInput
{
public:
    ${(function () {
        typeArr = [];
        varArr = [];
        QueryFrom.match(/[^{\}]+(?=})/g).map((element) => {
            typeArr.push(element);
        });

        InputList.split(' ').map((element) => {
            element = element.replace(',', '');
            varArr.push(element);
        });

        let text;
        for (let i in typeArr) {
            i == 0 ? (text += `${typeArr[i]} ${varArr[i]};\n`) : (text += `    ${typeArr[i]} ${varArr[i]};\n`);
        }
        text = text.replace('undefined', '');

        return text;
    })()}
public:
    ${QueryName}()
    {
        SetDBType(DB_TYPE::DB_${DBType});
        SetQueryType(QUERY_TYPE::${OutputList == '' ? 'WRITE' : 'READ'});
    }

    virtual std::string MakeQuery() override
    {
        ${(function () {
            const length = QueryFrom.match(/[^{\}]+(?=})/g).length;
            let query = `return ("${QueryFrom});`;
            for (let i = 0; i < length; i++) {
                let value = varArr[i];
                if (typeArr[i] != 'string') value = `std::to_string(${value})`;

                if (i != length - 1) query = query.replace(/{(.*?)}/, `" + ${value} + "`);
                else query = query.replace(/{(.*?)}/, `" + ${value}`);
            }
            return query;
        })()}
    }

    void SetParameter(${(function () {
        typeArr = [];
        varArr = [];
        QueryFrom.match(/[^{\}]+(?=})/g).map((element) => {
            typeArr.push(element);
        });

        InputList.split(' ').map((element) => {
            element = element.replace(',', '');
            varArr.push(element);
        });

        let text;
        for (let i in typeArr) {
            i == 0 ? (text += `${typeArr[i]} ${varArr[i]}`) : (text += `, ${typeArr[i]} ${varArr[i]}`);
        }
        text = text.replace('undefined', '');

        return text;
    })()})
    {
        ${(function () {
            varArr = [];

            InputList.split(' ').map((element) => {
                element = element.replace(',', '');
                varArr.push(element);
            });

            let text;
            for (let i in typeArr) {
                i == 0 ? (text += `this->${varArr[i]} = ${varArr[i]};`) : (text += `\n        this->${varArr[i]} = ${varArr[i]};`);
            }
            text = text.replace('undefined', '');

            return text;
        })()}
    }
};
                `;
        }

        fs.writeFileSync(QueryInputPath, InputQuery);
        fs.writeFileSync(QueryReturnPath, QueryReturn);

        console.log('Generate Finish!');
    });
