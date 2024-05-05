# Задание по программированию «Поисковая система»

Было бы странно в курсе от Яндекса и не попросить вас сделать свою поисковую систему. В финальной задаче «Красного пояса» мы именно этим и займёмся :)

В этой задаче вам надо поработать с классом SearchServer, который позволяет выполнять поиск в базе документов:

```C++
class SearchServer {
public:
SearchServer() = default;
explicit SearchServer(istream& document_input);

void UpdateDocumentBase(istream& document_input);
void AddQueriesStream(istream& query_input, ostream& search_results_output);
};
```

Рассмотрим его интерфейс.

## Конструктор

Конструктор класса SearchServer принимает поток ввода, содержащий базу документов. При этом

- один документ — это одна строка входного потока;
- документы состоят из слов, разделённых одним или несколькими пробелами;
- слова состоят из строчных латинских букв. Например, код, приведённый ниже, загружает в объект класса SearchServer базу из трёх документов:

```C++
string docs;
docs += "london is the capital of great britain\n";
// документ содержит 7 слов
docs += "i am travelling down the river\n";
// документ содержит 6 слов
docs += " here we go ";
// документ содержит 3 слова

istringstream document_input(docs);
SearchServer srv(document_input);
```

## Метод AddQueriesStream(istream& query_input, ostream& search_results_output)

Метод AddQueriesStream выполняет собственно поиск. Он принимает входной поток поисковых запросов и выходной поток для записи результатов поиска. При этом

- один запрос — это одна строка в потоке query_input
- каждый поисковый запрос состоит из слов, разделённых одним или несколькими пробелами
- так же, как и в документах, слова в запросах состоят из строчных латинских букв

Результатом обработки поискового запроса является набор из максимум пяти наиболее релевантных документов. В реальных поисковых системах метрика релевантности устроена довольно сложно. В рамках нашей задачи в качестве метрики релевантности мы будем использовать суммарное количество вхождений всех слов запроса в документ. Например, допустим, у нас есть поисковая база из трёх документов: "london is the capital of great britain", "moscow is the capital of the russian federation", "paris is the capital of france", — и поисковый запрос "the best capital". Тогда метрика релевантности у наших документов будет такой:

- london is the capital of great britain — 2 (слово "the" входит в документ 1 раз, слово "best" — ни разу, слово "capital" — 1 раз)
- moscow is the capital of the russian federation — 3 (слово "the" входит в документ 2 раза, слово "best" — ни разу, слово "capital" — 1 раз)
- paris is the capital of france — 2 ("the" — 1, "best" — 0, "capital" — 1)

В итоге получается, что документ "moscow is the capital of the russian federation" оказывается наиболее релевантным запросу "the best capital".

Для каждого поискового запроса метод AddQueriesStream должен вывести в поток search_results_output одну строку в формате [текст запроса]: {docid: <значение>, hitcount: <значение>} {docid: <значение>, hitcount: <значение>} ..., где docid — идентификатор документа (см. ниже), а hitcount — значение метрики релевантности для данного документа (то есть суммарное количество вхождений всех слов запроса в данный документ).

Два важных замечания:

- Добавлять в результаты поиска документы, hitcount которых равен нулю, не нужно.
- при подсчёте hitcount нужно учитывать только слова целиком, то есть слово «there» не является вхождением слова «the»

## Метод UpdateDocumentBase(istream& document_input)

Метод UpdateDocumentBase заменяет текущую базу документов на новую, которая содержится в потоке document_input. При этом документ из первой строки этого потока будет иметь идентификатор (docid) 0, документ из второй строки — идентификатор 1 и т.д. Точно так же должен назначать идентификаторы документам и конструктор класса SearchServer. Например, код

```C++
const string doc1 = "london is the capital of great britain";
const string doc2 = "moscow is the capital of the russian federation";
istringstream doc_input1(doc1 + '\n' + doc2);
SearchServer srv(doc_input1);

const string query = "the capital";
istringstream query_input1(query);
srv.AddQueriesStream(query_input1, cout);

istringstream doc_input2(doc2 + '\n' + doc1);
srv.UpdateDocumentBase(doc_input2);
istringstream query_input2(query);
srv.AddQueriesStream(query_input2, cout);
```

должен выводить

```bash
the capital: {docid: 1, hitcount: 3} {docid: 0, hitcount: 2}
the capital: {docid: 0, hitcount: 3} {docid: 1, hitcount: 2}
```

## Первая часть задачи

Эта задача состоит из двух частей. В первой части вам дана корректная реализация класса SearchServer, которая работает недостаточно быстро. Вам нужно найти и устранить узкие места в её реализации. В тестирующую систему нужно сдать cpp-файл или архив из нескольких исходных файлов, содержащий вашу ускоренную реализацию. Ваша реализация будет тестироваться вот такой функцией (объект класса SearchServer будет создан один раз, и у него один раз будет вызван метод AddQueriesStream):

```C++
void TestSearchServer(istream& document_input, istream& query_input,
ostream& search_results_output) {
SearchServer srv(document_input);
srv.AddQueriesStream(query_input, search_results_output);
}
```

При этом:

- document_input содержит не более 50000 документов
- каждый документ содержит не более 1000 слов
- общее число различных слов во всех документах не превосходит 10000
- максимальная длина одного слова — 100 символов, слова состоят из строчных латинских букв и разделены одним или несколькими пробелами
- query_input содержит не более 500 000 запросов, каждый запрос содержит от 1 до 10 слов.

Смотрите юнит-тесты в заготовке решения для более подробного ознакомления с тем, как должен работать класс SearchServer.

[course_project_first_part](https://stepik.org/media/attachments/lesson/285359/dO2HqHCSEeiMwApe4i-fLg_751bc350709211e8a2b191c246483517_course_project_first_part.zip)

### Подсказки

Для каждой части задачи есть набор подсказок, которые должны помочь вам с решением. Если вам не удаётся сдать задачу и вы чувствуете, что у вас кончились идеи, вы можете ими воспользоваться.

Подсказки для первой части

[подсказка_1.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_1.pdf)

[подсказка_2.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_2.pdf)

[подсказка_3.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_3.pdf)

[подсказка_4.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_4.pdf)

[подсказка_5.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_5.pdf)

[подсказка_6.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_6.pdf)

[подсказка_7.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_7.pdf)

[подсказка_8.pdf](https://stepik.org/media/attachments/lesson/285359/%D0%BF%D0%BE%D0%B4%D1%81%D0%BA%D0%B0%D0%B7%D0%BA%D0%B0_8.pdf)
