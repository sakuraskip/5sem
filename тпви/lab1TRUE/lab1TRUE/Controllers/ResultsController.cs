using Microsoft.AspNetCore.Mvc;
using BSTU.Results.Collection;
namespace lab1TRUE.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ResultsController : Controller
    {
        private BSTU.Results.Collection.Results _results;

        public ResultsController(BSTU.Results.Collection.Results results)
        {
            _results = results;
        }
        [HttpGet]
        public ActionResult GetAllResults()
        {
            var info = _results.GetAllResults();
            return Ok(info);
        }
        [HttpGet("{id}")]
        public ActionResult GetResultById(int id)
        {
            var info = _results.GetOneResults(id);
            if(info !=null || info != string.Empty)
            {
                return Ok(new { id, info });
            }
            return NotFound();
        }
        [HttpPost]
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
