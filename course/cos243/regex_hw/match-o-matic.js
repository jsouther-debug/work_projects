#!/usr/bin/env node

const ERR_STATUS = 'ERR';
const OK_STATUS = ' OK';

class RegexGrader {
  constructor(verbose=false) {
	this.verbose = verbose;
  }

  print(...args) {
	if (this.verbose) {
	  console.log(...args);
	}
  }
  
  scoreProblemFile(problemFilePath) {
	let total_matches = 0;
	let successful_matches = 0;
	let re_counter = 0;

	const problems = require(problemFilePath);

	problems.forEach(problem => {
	  this.print(`${++re_counter} RE ${problem.re}`);
	  problem.targets.forEach(target => {
		let result = problem.re.test(target.string);

		total_matches++;

		let status = ERR_STATUS;
		if (result == target.expected) {
		  status = OK_STATUS;
		  successful_matches++;
		}
		this.print(`  ${status} on '${target.string}': expected ${target.expected} got ${result}`);
	  });
	  this.print('-'.repeat(80));
	});

	return [successful_matches, total_matches];
  }
}

if (require.main === module) {
  let problemFile = './problems';
  console.log(process.argv);
  if (process.argv.length == 3) {
	problemFile = process.argv[2];
  }

  const grader = new RegexGrader(true);
  [successful_matches, total_matches] = grader.scoreProblemFile(problemFile);
  const percent = total_matches > 0 ? Math.floor(successful_matches / total_matches * 100.0) : 0;
  console.log(`${successful_matches}/${total_matches} (${percent}%)`);
} else {
  module.exports = RegexGrader;
}
