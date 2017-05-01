#!/usr/bin/python3

import wikipedia
import sys
import re

pretitle = "/wiki/"
pretitle_size = 6
useless_sections = {"See also", "References", "Sources", "Further reading", "External links"};
for line in sys.stdin:
	url = line.split('\t')
	index = url[0].find(pretitle)
	title_in_url = url[0][index + pretitle_size:]
	page_title = re.sub('_', ' ', title_in_url)
	#page_title = re.sub(r'\([^)]*\)', '', page_title)
	page = wikipedia.WikipediaPage(''.join(page_title))
	page_content = page.content
	page_content = re.sub('(\n|\t)', ' ', page_content)
	for section in useless_sections:
		ind_of_section = page_content.find("== " + section)
		if ind_of_section != -1:
			page_content = page_content[:ind_of_section]
	#page_content = re.sub(r'== .* ==', ' ', page_content)
	page_content = re.sub(r'[^\w]', ' ', page_content) 
	print(url[0] + '\t' + page_content)
