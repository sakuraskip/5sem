using Microsoft.AspNetCore.Mvc;
using BSTU.Results.Collection;
using AuthenticateResults;
using lab1TRUE.Models;
using Microsoft.AspNetCore.Authorization;
namespace lab1TRUE.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ResultsController : Controller
    {
        private BSTU.Results.Collection.Results _results;
        private Authenticate _authenticate;
        public ResultsController(BSTU.Results.Collection.Results results,Authenticate authenticate)
        {
            _results = results;
            _authenticate = authenticate;
        }
        [HttpPost("SignIn")]
        public async Task<ActionResult> SignIn([FromBody] UserProfile profile)
        {
            if(string.IsNullOrEmpty(profile.Username) || string.IsNullOrEmpty(profile.Password))
            {
                return BadRequest("username or password is empty");
            }
            var res = await _authenticate.SignInAsync(profile.Username, profile.Password);
            if(!res.Succeeded)
            {
                return NotFound();
            }
            return Ok(res);
        }
        [HttpGet("SignOut")]
        [Authorize]
        public async Task<ActionResult> SignOut()
        {
            await _authenticate.SignOut();
            return Ok("signed out");
        }
        [HttpGet]
        [Authorize(Roles = "READER")]
        public ActionResult GetAllResults()
        {
            var info = _results.GetAllResults();
            return Ok(info);
        }
        [HttpGet("{id}")]
        [Authorize(Roles = "READER")]
        public ActionResult GetResultById(int id)
        {
            string info = _results.GetOneResults(id);
            if(info != string.Empty)
            {
                return Ok(new { id, info });
            }
            return NotFound();
        }
        [HttpPost]
        [Authorize(Roles = "writer")]
        public ActionResult AddNewResult([FromBody] string value)
        {
            if(!string.IsNullOrEmpty(value))
            {
                var id = _results.AddOneResult(value);
                return Created($"{id}",new { id, value });
            }
            return BadRequest();
        }
        [HttpPut("{id}")]
        [Authorize(Roles = "writer")]
        public ActionResult UpdateResults(int id, [FromBody] string value)
        {
            if(!string.IsNullOrEmpty(value))
            {
                var flag = _results.UpdateOneResult(id, value);
                if(flag == -1)
                {
                    return BadRequest($"id {id} does not exists");
                }
                return NoContent();
            }
            return BadRequest();
        }
        [HttpDelete("{id}")]
        [Authorize(Roles = "writer")]
        public ActionResult DeleteResultById(int id)
        {
            if(_results.DeleteOneResult(id)!=-1)
            {
                return NoContent();
            }
            return NotFound();
        }
    }
}
