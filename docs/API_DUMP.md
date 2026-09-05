## Search API Request

Main request: `https://openlibrary.org/search.json`

Parameters:
* `q` - title
* `first_publish_year` - first publish year of a book
* `author` - author name
* `language` - language from ISO 639-2
* `sort` - sort mode (relevant, count of editions, older, newer, rating, etc.)

Example: `https://openlibrary.org/search.json?q=the+lord+of+the+rings&author=tolkien&sort=new`

## Search API Response

The following data about books can be extracted from the JSON response: `author_name`, `language`, `title`, `key` (used alongside the title to generate the full link), and `first_publish_year`.

Example response payload:
```json
{
  "author_key": ["..."],
  "author_name": [
    "J.R.R. Tolkien"
  ],
  "cover_edition_key": "...",
  "cover_i": 14625765,
  "ebook_access": "...",
  "edition_count": 252,
  "first_publish_year": 1954,
  "has_fulltext": true,
  "ia": ["..."],
  "ia_collection": ["..."],
  "key": "/works/OL27448W",
  "language": [
    "eng"
  ],
  "lending_edition_s": "...",
  "lending_identifier_s": "...",
  "public_scan_b": false,
  "series_key": ["..."],
  "series_name": ["..."],
  "series_position": ["..."],
  "title": "The Lord of the Rings"
}
```